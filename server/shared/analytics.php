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

require_once('datastore.php');
require_once('utils.php');

/** Command handler for the analytics interface. */
class Analytics
{

/** Schema check and update. */
public function get_check_schema()
{
    $db = new DataStore();
    $db->beginTransaction();
    $db->checkSchema();
    $db->commit();
}

/** List all products. */
public function get_products()
{
    $db = new DataStore();
    $products = $db->allProducts();

    $result = array();
    foreach ($products as $p) {
        $schema = $db->productSchema($p['id']);
        $p['schema'] = $schema;
        array_push($result, $p);
    }

    $json = json_encode($result);
    echo($json);
}

/** Add a new product. */
public function post_products()
{
    $rawPostData = file_get_contents('php://input');
    $product = json_decode($rawPostData, true);

    if ($product['name'] == "")
        Utils::httpError(400, "Product name is empty.");

    $db = new DataStore();
    $db->beginTransaction();
    $product['id'] = $db->addProduct($product);
    $db->updateProductSchema($product, $product['schema']);
    $db->commit();

    echo('Product ' . $product['name'] . " added.");
}

/** Update a given product. */
public function put_products($productId)
{
    $raw = file_get_contents('php://input');
    $productData = json_decode($raw, true);

    $db = new DataStore();
    $db->beginTransaction();
    $product = $db->productByName($productId);
    if (is_null($product))
        Utils::httpError(404, "Unknown product " . $productId . '.');

    $db->updateProductSchema($product, $productData['schema']);
    $db->commit();
    echo('Product ' . $productData['name'] . ' updated.');
}

/** Delete product and associated data. */
public function delete_products($productName)
{
    if ($productName == "")
        Utils::httpError(400, "Empty product name.");

    $db = new DataStore();
    $db->beginTransaction();
    $product = $db->productByName($productName);
    if (is_null($product))
        Utils::httpError(404, "Product not found.");

    $schema = $db->productSchema($product['id']);

    $db->deleteProduct($product, $schema);
    $db->commit();
    echo('Product ' . $productName . ' deleted.');
}

/** List data for a product. */
public function get_data($productName)
{
    $db = new DataStore();

    $product = $db->productByName($productName);
    if (is_null($product))
        Utils::httpError(404, "Unknown product.");
    $schema = $db->productSchema($product['id']);
    $productTableName = Utils::tableNameForProduct($product['name']);

    $basicEntries = array();
    foreach ($schema as $entry) {
        switch ($entry['type']) {
            case 'int':
            case 'string':
                $basicEntries[$entry['name']] = $entry;
                break;
        }
    }
    $data = $db->basicRecords($productTableName, $basicEntries);

    foreach ($schema as $entry) {
        $complexRecords = NULL;
        $complexTableName = Utils::tableNameForComplexEntry($product['name'], $entry['name']);
        switch ($entry['type']) {
            case 'string_list':
                $complexRecords = $db->stringListRecords($complexTableName);
                break;
            case 'ratio_set':
                $complexRecords = $db->ratioSetRecords($complexTableName);
                break;
        }
        if (is_null($complexRecords))
            continue;
        foreach ($data as &$row) {
            if (!array_key_exists($row['id'], $complexRecords))
                continue;
            $row[$entry['name']] = $complexRecords[$row['id']];
        }
    }

    echo(json_encode($data));
}

/** List all surveys for a product. */
public function get_surveys($product)
{
    if ($product == "")
        Utils::httpError(400, "No product id specified.");

    $db = new DataStore();
    $res = $db->surveysByProductName($product);
    for ($i = 0; $i < sizeof($res); $i++) {
        $res[$i]['active'] = $res[$i]['active'] == "1"; // fixup sqlite bool representation
    }
    echo(json_encode($res));
}

/** Add new survey. */
public function post_surveys($product)
{
    if ($product == "")
        Utils::httpError(400, "No product id specified.");

    $rawPostData = file_get_contents('php://input');
    $survey= json_decode($rawPostData, true);

    $db = new DataStore();
    $db->beginTransaction();
    $productData = $db->productByName($product);
    if (is_null($productData))
        Utils::httpError(404, "Invalid product identifier.");

    $db->addSurvey($productData['id'], $survey);
    $db->commit();
    echo("Survey created for product $product.");
}

/** Edit an existing survey. */
public function put_surveys($surveyId)
{
    $surveyId = intval($surveyId);
    if ($surveyId < 0)
        Utils::httpError(400, "Invalid survey id.");

    $surveyData = file_get_contents('php://input');
    $surveyData = json_decode($surveyData, true);

    $db = new DataStore();
    $db->beginTransaction();
    $db->updateSurvey($surveyId, $surveyData);
    $db->commit();
    echo("Survery updated.");
}

/** Delete survey. */
public function delete_surveys($survey)
{
    $db = new DataStore();
    $db->beginTransaction();
    $db->deleteSurvey($survey);
    $db->commit();
    echo("Survey deleted.");
}

}

?>
