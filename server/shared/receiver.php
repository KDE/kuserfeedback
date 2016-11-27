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
require_once('restexception.php');
require_once('utils.php');
require_once('sample.php');
require_once('survey.php');

/** Command handler for the data receiver. */
class Receiver
{

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
        throw RESTException('Unknown product.', 404);

    Sample::insert($db, $rawPostData, $product);

    // read survey from db
    $responseData = array();
    $surveys = Survey::activeSurveysForProduct($db, $product);
    $responseData['surveys'] = $surveys;
    $db->commit();

    echo(json_encode($responseData));
}

}

?>
