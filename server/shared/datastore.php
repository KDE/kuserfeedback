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

// TODO temporary, make private
public $db;

function __construct()
{
    try {
        $this->db = new PDO(Config::dsn());
    } catch (PDOException $e) {
        die('Database connection failed: ' . $e->getMessage());
    }
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
    $this->db->beginTransaction();
    for ($i = $currentVersion; $i < $targetVersion; $i++) {
        echo("Applying update $i...");
        $this->applySchemaChange($schemaDefs['schema'][$i]);
    }
    $this->db->commit();
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
        if ($res === FALSE)
            $this->fatalDbError();
    }
    $res = $this->db->exec('UPDATE schema_version SET version = ' . $schemaDef['version']);
    if ($res === FALSE)
        $this->fatalDbError();
}

/** Prints last database error and dies. */
private function fatalDbError()
{
    print_r($this->db->errorInfo());
    die('Fatal Database Error.');
}

/** List all products. */
public function allProducts()
{
    $products = array();
    $res = $this->db->query('SELECT * FROM products');
    if ($res === FALSE)
        $this->fatalDbError();
    foreach ($res as $row)
        array_push($products, $row);
    return $products;
}

/** Add a new product. */
public function addProduct($product)
{
    // create product entry
    $res = $this->db->exec('INSERT INTO products (name) VALUES (' . $this->db->quote($product['name']) . ')');
    if ($res === FALSE)
        $this->fatalDbError();

    // create product table
    $tableName = Utils::tableNameForProduct($product['name']);
    $res = $this->db->exec('CREATE TABLE ' . $tableName . ' ('
        . 'id INTEGER PRIMARY KEY AUTOINCREMENT, '
        . 'productId INTEGER REFERENCES products (id), '
        . 'timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP, '
        . 'version VARCHAR, '
        . 'platform VARCHAR, '
        . 'startCount INTEGER, '
        . 'usageTime INTEGER'
        . ')');
    if ($res === FALSE)
        $this->fatalDbError();
}

/** Delete a product. */
public function deleteProduct($product)
{
    $res = $this->db->exec('DELETE FROM products WHERE name = ' . $this->db->quote($product));
    if ($res === FALSE)
        $this->fatalDbError();

    $res = $this->db->exec('DROP TABLE ' . Utils::tableNameForProduct($product));
    if ($res === FALSE)
        $this->fatalDbError();
}

/** Look up product by name. */
public function productByName($name)
{
    $res = $this->db->query('SELECT * FROM products WHERE name = ' . $this->db->quote($name));
    if ($res === FALSE)
        $this->fatalDbError();
    foreach ($res as $row)
        return $row;
    return NULL;
}

/** All data for the give product name. */
public function rawDataForProduct($name)
{
    $tableName = Utils::tableNameForProduct($name);
    $res = $this->db->query('SELECT * FROM ' . $tableName);
    if ($res === FALSE)
        $this->fatalDbError();
    $data = array();
    foreach ($res as $row)
        array_push($data, $row);
    return $data;
}

}

?>
