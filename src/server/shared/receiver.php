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
require_once('restexception.php');
require_once('utils.php');
require_once('sample.php');
require_once('survey.php');

/** Command handler for the data receiver. */
class Receiver
{

/** Dummy command for probing for redirects. */
function get_submit($productName)
{
}

/** Data submission command. */
function post_submit($productName)
{
    // load JSON data sent by the client
    $rawPostData = file_get_contents('php://input');

    // look up product
    $db = new DataStore();
    $db->beginTransaction();
    $product = Product::productByName($db, $productName);
    if (is_null($product))
        throw new RESTException('Unknown product.', 404);

    Sample::insert($db, $rawPostData, $product);

    // read survey from db
    $responseData = array();
    $surveys = Survey::activeSurveysForProduct($db, $product);
    $responseData['surveys'] = $surveys;
    $db->commit();

    header('Content-Type: text/json');
    echo(json_encode($responseData));
}

}

?>
