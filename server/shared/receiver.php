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
include_once('utils.php');

/** Command handler for the data receiver. */
class Receiver
{

/** Data submission command. */
function post_submit()
{
    // load JSON data sent by the client
    $rawPostData = file_get_contents('php://input');
    $data = json_decode($rawPostData, true);

    // look up product
    $db = new DataStore();
    $db->beginTransaction();
    $product = $db->productByName($data['productId']);
    if (is_null($product))
        die("Unknown product.");
    $productSchema = $db->productSchema($product['id']);

    // write basic record
    $tableName = Utils::tableNameForProduct($product['name']);
    $basicData = array();
    foreach ($productSchema as $entry) {
        if (!array_key_exists($entry['name'], $data))
            continue;
        switch($entry['type']) {
            case 'string':
                $basicData[$entry['name']] = $data[$entry['name']];
                break;
            case 'int':
                $basicData[$entry['name']] = intval($data[$entry['name']]);
                break;
        }
    }
    $recordId = $db->addBasicRecord($tableName, $basicData);

    // add complex data to sub-tables
    foreach ($productSchema as $entry) {
        if (!array_key_exists($entry['name'], $data))
            continue;
        $tableName = Utils::tableNameForComplexEntry($product['name'], $entry['name']);
        switch($entry['type']) {
            case 'string_list':
                $db->addStringListRecord($tableName, $recordId, $data[$entry['name']]);
                break;
        }
    }

    // read survey from db
    $responseData = array();
    $surveys = $db->activeSurveysForProduct($product);
    if (sizeof($surveys) > 0) {
        $surveyInfo = array();
        $surveyInfo['id'] = intval($surveys[0]['id']);
        $surveyInfo['url'] = $surveys[0]['url'];
        $responseData['survey'] = $surveyInfo;
    }

    $db->commit();
    echo(json_encode($responseData));
}

}

?>
