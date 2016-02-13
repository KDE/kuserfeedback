<?php
/*
    Copyright (C) 2016 Volker Krause <vkrause@kde.org>

    This program is free software; you can redistribute it and/or modify it
    under the terms of the GNU Library General Public License as published by
    the Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
    License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

include_once('../config/config.php');
include_once('utils.php');

/** Handles all database operations. */
class DataStore {

private $db;

function __construct()
{
    try {
        $this->db = new PDO(Config::dsn());
    } catch (PDOException $e) {
        die('Database connection failed: ' . $e->getMessage());
    }
}

/** Check database result for errors, and if so, bail out. */
private function checkError($res)
{
    if ($res === FALSE) {
        http_response_code(500);
        header('Content-Type: text/plain');
        print_r($this->db->errorInfo());
        exit(1);
    }
}

/** Begin transaction. */
public function beginTransaction()
{
    $this->db->beginTransaction();
}

/** Commit transaction. */
public function commit()
{
    $this->db->commit();
}

/** Verify the database schema, and fix if needed. */
public function checkSchema()
{
    $currentVersion = $this->schemaVersion();
    $schemaFile = __DIR__ . '/schema.json';
    $schemaDefs = json_decode(file_get_contents($schemaFile), true);
    $targetVersion = count($schemaDefs['schema']);

    if ($currentVersion == $targetVersion) {
        echo('Schema is up-to-date.');
        return;
    }

    if ($currentVersion > $targetVersion || $currentVersion < 0)
        die('Current schema version is invalid: ' . $currentVersion . '.');

    # apply database updates
    echo('Current schema version: ' . $currentVersion . ' should be: ' . $targetVersion);
    for ($i = $currentVersion; $i < $targetVersion; $i++) {
        echo("Applying update $i...");
        $this->applySchemaChange($schemaDefs['schema'][$i]);
    }
}

/** Returns the current schema version. */
private function schemaVersion()
{
    foreach ($this->db->query('SELECT version FROM schema_version') as $row) {
        return $row['version'];
    }
    return 0;
}

/** Applies a list of schema setup commands. */
private function applySchemaChange($schemaDef)
{
    foreach($schemaDef['sql'] as $cmd) {
        $res = $this->db->exec($cmd);
        $this->checkError($res);
    }
    $res = $this->db->exec('UPDATE schema_version SET version = ' . $schemaDef['version']);
    $this->checkError($res);
}

/** List all products. */
public function allProducts()
{
    $products = array();
    $res = $this->db->query('SELECT * FROM products');
    $this->checkError($res);
    foreach ($res as $row)
        array_push($products, $row);
    return $products;
}

/** Add a new product. */
public function addProduct($product)
{
    // create product entry
    $res = $this->db->exec('INSERT INTO products (name) VALUES (' . $this->db->quote($product['name']) . ')');
    $this->checkError($res);

    // create product table
    $tableName = Utils::tableNameForProduct($product['name']);
    $res = $this->db->exec('CREATE TABLE ' . $tableName . ' ('
        . 'id INTEGER PRIMARY KEY AUTOINCREMENT, '
        . 'timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP)'
    );
    $this->checkError($res);

    return $this->db->lastInsertId();
}

/** Delete a product. */
public function deleteProduct($product, $schema)
{
    foreach ($schema as $entry) {
        switch ($entry['type']) {
            case "string_list":
                $this->deleteComplexSchemaEntry($product, $entry);
                break;
        }
    }

    $res = $this->db->exec('DELETE FROM product_schema WHERE productId = ' . intval($product['id']));
    $this->checkError($res);

    $res = $this->db->exec('DELETE FROM products WHERE id = ' . intval($product['id']));
    $this->checkError($res);

    $res = $this->db->exec('DROP TABLE ' . Utils::tableNameForProduct($product['name']));
    $this->checkError($res);
}

/** Look up product by name. */
public function productByName($name)
{
    $res = $this->db->query('SELECT * FROM products WHERE name = ' . $this->db->quote($name));
    $this->checkError($res);
    foreach ($res as $row)
        return $row;
    return NULL;
}

/** Get the schema of product $productId. */
public function productSchema($productId)
{
    $res = $this->db->query('SELECT * FROM product_schema WHERE productId = ' . intval($productId));
    $this->checkError($res);

    $schema = array();
    foreach ($res as $row) {
        $entry = array();
        $entry['name'] = $row['name'];
        $entry['type'] = $row['type'];
        array_push($schema, $entry);
    }
    return $schema;
}

/** Update product schema to $schema. */
public function updateProductSchema($product, $schema)
{
    $oldSchema = array();
    foreach ($this->productSchema($product['id']) as $o) {
        $oldSchema[$o['name']] = $o;
    }

    foreach ($schema as $entry) {
        if (array_key_exists($entry['name'], $oldSchema)) {
            // update
            $oldType = $oldSchema[$entry['name']]['type'];
            if ($oldType != $entry['type'])
                die('Cannot change data type of entry ' . $entry['name'] . '!');

            $res = $this->db->exec('UPDATE product_schema SET ' .
                'type = ' . $this->db->quote($entry['type']) . ' WHERE ' .
                'productId = ' . intval($product['id']) . ' AND ' .
                'name = ' . $this->db->quote($entry['name'])
            );
            $this->checkError($res);
        } else {
            // insert
            $res = $this->db->exec('INSERT INTO product_schema (productId, name, type) VALUES (' .
                intval($product['id']) . ', ' .
                $this->db->quote($entry['name']) . ', ' .
                $this->db->quote($entry['type']) . ')'
            );
            $this->checkError($res);

            $this->addProductSchemaEntry($product, $entry);
        }

        unset($oldSchema[$entry['name']]);
    }

    // delete whatever is left
    foreach($oldSchema as $entry) {
        $res = $this->db->exec('DELETE FROM product_schema WHERE ' .
            'productId = ' . intval($product['id']) . ' AND ' .
            'name = ' . $this->db->quote($entry['name'])
        );
        $this->checkError($res);

        $this->deleteProductSchemaEntry($product, $entry);
    }
}

/** Add a database elements needed for a given product schema entry. */
private function addProductSchemaEntry($product, $entry)
{
    $productTable = Utils::tableNameForProduct($product['name']);

    switch ($entry['type']) {
        case "string":
            $res = $this->db->exec('ALTER TABLE ' . $productTable . ' ADD COLUMN ' . $entry['name'] . ' VARCHAR');
            $this->checkError($res);
            break;
        case "int":
            $res = $this->db->exec('ALTER TABLE ' . $productTable . ' ADD COLUMN ' . $entry['name'] . ' INTEGER');
            $this->checkError($res);
            break;
        case "string_list":
        {
            $tableName = Utils::tableNameForComplexEntry($product['name'], $entry['name']);
            $res = $this->db->exec('CREATE TABLE ' . $tableName . ' ('
                . 'id INTEGER PRIMARY KEY AUTOINCREMENT, '
                . 'sampleId INTEGER REFERENCES ' . $productTable . '(id), '
                . 'value VARCHAR)'
            );
            $this->checkError($res);
            break;
        }
    }
}

/** Delete database elements for a given product schema entry. */
private function deleteProductSchemaEntry($product, $entry)
{
    $productTable = Utils::tableNameForProduct($product['name']);

    switch ($entry['type']) {
        case "string":
        case "int":
            $res = $this->db->exec('ALTER TABLE ' . $productTable . ' DROP COLUMN ' . $entry['name']);
            // FIXME: DROP COLUMN does not work with sqlite
            //$this->checkError($res);
            break;
        case "string_list":
            $this->deleteComplexSchemaEntry($product, $entry);
            break;
    }
}

/** Delete database elements for a complex product schema entry. */
private function deleteComplexSchemaEntry($product, $entry)
{
    $tableName = Utils::tableNameForComplexEntry($product['name'], $entry['name']);
    $res = $this->db->exec("DROP TABLE " . $tableName);
    $this->checkError($res);
}

/** All data for the give product table and schema entries. */
public function basicRecords($productTableName, $basicEntries)
{
    $res = $this->db->query('SELECT timestamp, ' . implode(', ', array_keys($basicEntries)) . ' FROM ' . $productTableName);
    $this->checkError($res);
    $data = array();
    foreach ($res as $row) {
        $sample = array();
        $sample['timestamp'] = $row['timestamp'];
        foreach ($basicEntries as $col) {
            switch ($col['type']) {
                case 'int':
                    $sample[$col['name']] = intval($row[$col['name']]);
                    break;
                case 'string':
                    $sample[$col['name']] = $row[$col['name']];
                    break;
            }
        }
        array_push($data, $sample);
    }
    return $data;
}

/** Add a new record into the given product data table. */
public function addBasicRecord($productTable, $data)
{
    $quotedData = array();
    foreach(array_keys($data) as $key) {
        if (is_string($data[$key]))
            array_push($quotedData, $this->db->quote($data[$key]));
        else
            array_push($quotedData, $data[$key]);
    }
    $res = $this->db->exec('INSERT INTO ' . $productTable . ' ('
        . implode(', ', array_keys($data)) . ') VALUES ('
        . implode(', ', $quotedData) . ')'
    );
    $this->checkError($res);
    return $this->db->lastInsertId();
}

/** List all survey for a product name. */
public function surveysByProductName($product)
{
    $res = $this->db->query('SELECT surveys.* FROM surveys JOIN products ON (surveys.productId = products.id) WHERE products.name = ' . $this->db->quote($product));
    $this->checkError($res);
    $surveys = array();
    foreach ($res as $row)
        array_push($surveys, $row);
    return $surveys;
}

/** List all active surveys for a given product. */
public function activeSurveysForProduct($product)
{
    $res = $this->db->query('SELECT * FROM surveys WHERE productId = ' . $product['id'] . ' AND active = 1');
    $this->checkError($res);
    $surveys = array();
    foreach ($res as $row)
        array_push($surveys, $row);
    return $surveys;
}

/** Add a new survey for a product given by id. */
public function addSurvey($productId, $survey)
{
    $res = $this->db->exec('INSERT INTO surveys (productId, name, url) VALUES ('
        . $productId . ', '
        . $this->db->quote($survey['name']) . ', '
        . $this->db->quote($survey['url']) . ')');
    $this->checkError($res);
}

/** Update an existing survey with id @p $surveyId. */
public function updateSurvey($surveyId, $surveyData)
{
    $res = $this->db->exec('UPDATE surveys SET'
        . ' name = ' . $this->db->quote($surveyData['name']) . ','
        . ' url = ' . $this->db->quote($surveyData['url']) . ','
        . ' active = ' . ($surveyData['active'] ? 1 : 0)
        . ' WHERE id = ' . $surveyId);
    $this->checkError($res);
}

/** Delete a survey given its internal id. */
public function deleteSurvey($surveyId)
{
    $res = $this->db->exec('DELETE FROM surveys WHERE id = ' . $this->db->quote($surveyId));
    $this->checkError($res);
}

}

?>
