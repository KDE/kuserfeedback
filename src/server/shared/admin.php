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
require_once('product.php');
require_once('sample.php');
require_once('survey.php');
require_once('utils.php');

/** Command handler for the admin interface. */
class Admin
{

/** Add a new product. */
public function post_products()
{
    $rawPostData = file_get_contents('php://input');
    $product = Product::fromJson($rawPostData);

    $db = new DataStore();
    $db->beginTransaction();
    $product->insert($db);
    $db->commit();

    echo('Product ' . $product->name . " added.");
}

/** Update a given product. */
public function put_products($productName)
{
    $raw = file_get_contents('php://input');
    $newProduct = Product::fromJson($raw);

    $db = new DataStore();
    $db->beginTransaction();
    $oldProduct = Product::productByName($db, $productName);
    if (is_null($oldProduct))
        throw new RESTException('Product not found.', 404);

    $oldProduct->update($db, $newProduct);
    $db->commit();
    echo('Product ' . $productName . ' updated.');
}

/** Delete product and associated data. */
public function delete_products($productName)
{
    $db = new DataStore();
    $db->beginTransaction();
    $product = Product::productByName($db, $productName);
    if (is_null($product))
        throw new RESTException('Product not found.', 404);
    $product->delete($db);
    $db->commit();
    echo('Product ' . $productName . ' deleted.');
}

/** Import data for a product. */
public function post_data($productName)
{
    $db = new DataStore();
    $db->beginTransaction();
    $product = Product::productByName($db, $productName);
    if (is_null($product))
        throw RESTException('Unknown product.', 404);
    Sample::import($db, file_get_contents('php://input'), $product);
    $db->commit();
    echo('Data imported.');
}

/** Add new survey. */
public function post_surveys($productName)
{
    if ($productName == "")
        Utils::httpError(400, "No product id specified.");

    $rawPostData = file_get_contents('php://input');
    $survey = Survey::fromJson($rawPostData);

    $db = new DataStore();
    $db->beginTransaction();
    $product = Product::productByName($db, $productName);
    if (is_null($product))
        Utils::httpError(404, "Invalid product identifier.");

    $survey->insert($db, $product);
    $db->commit();
    echo('Survey created for product ' . $product->name . '.');
}

/** Edit an existing survey. */
public function put_surveys($surveyId)
{
    $surveyId = strval($surveyId);
    if (strlen($surveyId) <= 0)
        throw new RESTException('Invalid survey id.', 400);

    $surveyData = file_get_contents('php://input');
    $survey = Survey::fromJson($surveyData);
    $survey->uuid = $surveyId;

    $db = new DataStore();
    $db->beginTransaction();
    $survey->update($db);
    $db->commit();
    echo("Survey updated.");
}

/** Delete survey. */
public function delete_surveys($surveyId)
{
    $survey = new Survey;
    $survey->uuid = strval($surveyId);
    if (strlen($survey->uuid) <= 0)
        throw new RESTException('Invalid survey id.', 400);

    $db = new DataStore();
    $db->beginTransaction();
    $survey->delete($db);
    $db->commit();
    echo("Survey deleted.");
}

}

?>
