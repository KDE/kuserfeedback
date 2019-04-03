<?php
/*
    Copyright (C) 2016 Volker Krause <vkrause@kde.org>

    Permission is hereby granted, free of charge, to any person obtaining
    a copy of this software and associated documentation files (the
    "Software"), to deal in the Software without restriction, including
    without limitation the rights to use, copy, modify, merge, publish,
    distribute, sublicense, and/or sell copies of the Software, and to
    permit persons to whom the Software is furnished to do so, subject to
    the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

require_once('datastore.php');
require_once('product.php');
require_once('sample.php');
require_once('survey.php');
require_once('utils.php');

/** Command handler for the analytics interface. */
class Analytics
{

/** Schema check and update. */
public function get_check_schema()
{
    // check PHP version
    if (PHP_VERSION_ID < 50400)
        throw new RESTException('Minium required PHP version is 5.4, found ' . PHP_VERSION, 500);

    // check database layout
    $db = new DataStore();
    $res = $db->checkSchema();
    $res['protocolVersion'] = 2;

    header('Content-Type: text/json');
    echo(json_encode($res));
}

/** List all products. */
public function get_products()
{
    $db = new DataStore();
    $products = Product::allProducts($db);
    $json = json_encode($products);

    header('Content-Type: text/json');
    echo($json);
}

/** List data for a product. */
public function get_data($productName)
{
    $db = new DataStore();
    $product = Product::productByName($db, $productName);
    if (is_null($product))
        throw new RESTException('Product not found.', 404);

    header('Content-Type: text/json');
    echo(Sample::dataAsJson($db, $product));
}

/** List all surveys for a product. */
public function get_surveys($productName)
{
    if ($productName == "")
        Utils::httpError(400, "No product id specified.");

    $db = new DataStore();
    $surveys = Survey::surveysForProduct($db, $productName);

    header('Content-Type: text/json');
    echo(json_encode($surveys));
}

}

?>
