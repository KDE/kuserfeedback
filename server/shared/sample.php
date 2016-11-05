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
require_once('schemaentry.php');
require_once('schemaentryelement.php');

/** Represents a data sample received from a user. */
class Sample
{

    /** Insert a received sample for @p product into the data store. */
    public static function insert(Datastore $db, $jsonData, Product $product)
    {
        $jsonObj = json_decode($jsonData);
        $sampleId = self::insertScalar($db, $jsonObj, $product);
    }

    /** Insert scalar data for @p product.
     *  @return The sample id for use in non-scalar data tables.
     */
    private static function insertScalar(Datastore $db, $jsonObj, Product $product)
    {
        $columns = array();
        $binds = array();
        $values = array();

        foreach ($product->schema as $entry) {
            if (!$entry->isScalar())
                continue;
            if (!property_exists($jsonObj, $entry->name))
                continue;
            foreach ($entry->elements as $elem) {
                if (!property_exists($jsonObj->{$entry->name}, $elem->name))
                    continue;

                $v = $jsonObj->{$entry->name}->{$elem->name};
                switch ($entry->type) {
                    case SchemaEntryElement::STRING_TYPE:
                        if (!is_string($v))
                            continue;
                        break;
                    case SchemaEntryElement::INT_TYPE:
                        if (!is_int($v))
                            continue;
                        break;
                    case SchemaEntryElement::NUMBER_TYPE:
                        if (!is_float($v))
                            continue;
                        break;
                    case SchemaEntryElement::BOOL_TYPE:
                        if (!is_bool($v))
                            continue;
                        break;
                    default:
                        continue;
                }

                $bind = ':' . $elem->name;
                array_push($columns, $elem->name);
                array_push($binds, $bind);
                $values[$bind] = $v;
            }
        }

        $sql = 'INSERT INTO ' . $product->dataTableName();
        if (count($columns) > 0) {
            $sql .= ' (' . implode(', ', $columns) . ') VALUES (';
            $sql .= implode(', ', $binds) . ')';
        }
        $stmt = $db->prepare($sql);
        $db->execute($stmt, $values);

        return $db->pdoHandle()->lastInsertId();
    }
}

?>

