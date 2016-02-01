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

include_once('datastore.php');

/** Command handler for the analytics interface. */
class Analytics
{

/** Schema check and update. */
public function get_check_schema()
{
    $db = new DataStore();
    $db->checkSchema();
}

/** List all products. */
public function get_products()
{
    $db = new DataStore();
    $products = $db->allProducts();
    $json = json_encode($products);
    echo($json);
}

/** Add a new product. */
public function post_products()
{
    $rawPostData = file_get_contents('php://input');
    $product = json_decode($rawPostData, true);

    if ($product['name'] == "")
        die("Product name is empty.");

    $db = new DataStore();
    $db->addProduct($product);
    echo('Product ' . $product['name'] . " added.");
}

/** Delete product and associated data. */
public function delete_products($product)
{
    if ($product == "")
        die("Empty product name.");

    $db = new DataStore();
    $db->deleteProduct($product);
    echo('Product ' . $product . ' deleted.');
}

/** List data for a product. */
public function get_data($product)
{
    $db = new DataStore();
    $data = $db->rawDataForProduct($product);
    echo(json_encode($data));
}

/** List all surveys for a product. */
public function get_surveys($product)
{
    if ($product == "")
        die("No product id specified.");

    $db = new DataStore();
    $res = $db->surveysByProductName($product);
    echo(json_encode($res));
}

/** Add new survey. */
public function post_surveys($product)
{
    if ($product == "")
        die("No product id specified.");

    $rawPostData = file_get_contents('php://input');
    $survey= json_decode($rawPostData, true);

    $db = new DataStore();
    $productData = $db->productByName($product);
    if (is_null($productData))
        die("Invalid product identifier.");

    $db->addSurvey($productData['id'], $survey);
    echo("Survey created for product $product.");
}

/** Delete survey. */
public function delete_surveys($survey)
{
    $db = new DataStore();
    $db->deleteSurvey($survey);
    echo("Survey deleted.");
}

}

?>
