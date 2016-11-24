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

require_once('../../server/shared/datastore.php');
require_once('../../server/shared/product.php');

class ProductTest extends PHPUnit_Extensions_Database_TestCase
{
    static private $db = null;
    private $conn = null;

    public function getConnection()
    {
        if (is_null($this->conn)) {
            if (is_null(self::$db)) {
                self::$db = new Datastore('sqlite::memory:');
                self::$db->checkSchema();
            }
            $this->conn = $this->createDefaultDBConnection(self::$db->pdoHandle(), ':memory:');
        }
        return $this->conn;
    }

    public function getDataSet()
    {
        return $this->createXmlDataSet(__DIR__ . '/dataset.xml');
    }

    public function testFromJson()
    {
        $p = Product::fromJson('{
            "name": "org.kde.product",
            "schema": [
                {
                    "name": "entry1",
                    "type": "scalar",
                    "aggregationType": "category",
                    "elements" : [
                        { "name": "element1", "type": "int" },
                        { "name": "element2", "type": "bool" }
                    ]
                }
            ]
        }');
        $this->assertEquals($p->name, 'org.kde.product');
        $this->assertEquals(1, count($p->schema));
        $this->assertEquals('product_org_kde_product', $p->dataTableName());
        $entry = $p->schema[0];
        $this->assertEquals($entry->name, 'entry1');
        $this->assertEquals($entry->type, 'scalar');
        $this->assertEquals($entry->aggregationType, 'category');

        $this->assertEquals(2, count($entry->elements));
        $elem = $entry->elements[1];
        $this->assertInstanceOf(SchemaEntryElement::class, $elem);
        $this->assertEquals('element2', $elem->name);
        $this->assertEquals('bool', $elem->type);
    }

    public function testToJson()
    {
        $p = new Product();
        $p->name = 'org.kde.product';
        $entry = new SchemaEntry($p);
        $entry->name = 'entry1';
        $entry->type = 'list';
        $entry->aggregationType = 'numeric';
        $elem = new SchemaEntryElement($entry);
        $elem->name = 'element3';
        $elem->type = 'number';
        array_push($entry->elements, $elem);
        array_push($p->schema, $entry);

        $this->assertJsonStringEqualsJsonString(json_encode($p), '{
            "name": "org.kde.product",
            "schema": [
                {
                    "name": "entry1",
                    "type": "list",
                    "aggregationType": "numeric",
                    "elements" : [
                        { "name": "element3", "type": "number" }
                    ]
                }
            ]
        }');
    }

    public function testAllProducts()
    {
        $ps = Product::allProducts(self::$db);
        $this->assertEquals(1, count($ps));
        $p = $ps[0];
        $this->assertInstanceOf(Product::class, $p);
        $this->assertEquals('org.kde.UnitTest', $p->name);
        $this->assertEquals(2, count($p->schema));

        $entry1 = $p->schema[0];
        $entry2 = $p->schema[1];
        if ($entry1->name > $entry2->name) {
            $entry1 = $p->schema[1];
            $entry2 = $p->schema[0];
        }

        $this->assertInstanceOf(SchemaEntry::class, $entry1);
        $this->assertEquals($entry1->name, 'entry1');
        $this->assertEquals($entry1->type, 'scalar');
        $this->assertEquals($entry1->aggregationType, 'category');
        $this->assertEquals(2, count($entry1->elements));
        $elem = $entry1->elements[1];
        $this->assertInstanceOf(SchemaEntryElement::class, $elem);
        $this->assertEquals('element12', $elem->name);
        $this->assertEquals('bool', $elem->type);

        $this->assertInstanceOf(SchemaEntry::class, $entry2);
        $this->assertEquals($entry2->name, 'entry2');
        $this->assertEquals($entry2->type, 'list');
        $this->assertEquals($entry2->aggregationType, 'xy');
    }

    public function testProductByName()
    {
        $p = Product::productByName(self::$db, 'I don\'t exist');
        $this->assertNull($p);

        $p = Product::productByName(self::$db, 'org.kde.UnitTest');
        $this->assertInstanceOf(Product::class, $p);
        $this->assertEquals('org.kde.UnitTest', $p->name);
        $this->assertEquals(2, count($p->schema));

        $entry1 = $p->schema[0];
        $entry2 = $p->schema[1];
        if ($entry1->name > $entry2->name) {
            $entry1 = $p->schema[1];
            $entry2 = $p->schema[0];
        }

        $this->assertInstanceOf(SchemaEntry::class, $entry1);
        $this->assertEquals($entry1->name, 'entry1');
        $this->assertEquals($entry1->type, 'scalar');
        $this->assertEquals($entry1->aggregationType, 'category');
        $elem = $entry1->elements[1];
        $this->assertInstanceOf(SchemaEntryElement::class, $elem);
        $this->assertEquals('element12', $elem->name);
        $this->assertEquals('bool', $elem->type);

        $this->assertInstanceOf(SchemaEntry::class, $entry2);
        $this->assertEquals($entry2->name, 'entry2');
        $this->assertEquals($entry2->type, 'list');
        $this->assertEquals($entry2->aggregationType, 'xy');
    }

    public function testProductInsert()
    {
        $json = '{
            "name": "org.kde.NewProduct",
            "schema": [
                {
                    "name": "entryA",
                    "type": "scalar",
                    "aggregationType": "category",
                    "elements" : [
                        { "name": "elementA1", "type": "int" },
                        { "name": "elementA2", "type": "bool" }
                    ]
                },
                {
                    "name": "entryB",
                    "type": "map",
                    "aggregationType": "ratio_set",
                    "elements" : [
                        { "name": "elementB1", "type": "number" },
                        { "name": "elementB2", "type": "string" }
                    ]
                }
            ]
        }';

        $p = Product::fromJson($json);
        $p->insert(self::$db);

        $p = Product::productByName(self::$db, 'org.kde.NewProduct');
        $this->assertJsonStringEqualsJsonString($json, json_encode($p));
    }

    public function testProductUpdate()
    {
        $jsonOld = '{
            "name": "org.kde.ProductToUpdate",
            "schema": [
                {
                    "name": "entryA",
                    "type": "scalar",
                    "aggregationType": "none",
                    "elements" : [
                        { "name": "elementA1", "type": "int" },
                        { "name": "elementA2", "type": "bool" }
                    ]
                },
                {
                    "name": "entryB",
                    "type": "list",
                    "aggregationType": "ratio_set",
                    "elements" : [
                        { "name": "elementB1", "type": "number" },
                        { "name": "elementB2", "type": "string" }
                    ]
                }
            ]
        }';

        $p = Product::fromJson($jsonOld);
        $p->insert(self::$db);

        $p = Product::productByName(self::$db, 'org.kde.ProductToUpdate');
        $this->assertJsonStringEqualsJsonString($jsonOld, json_encode($p));

        $jsonNew = '{
            "name": "org.kde.ProductToUpdate",
            "schema": [
                {
                    "name": "entryA",
                    "type": "scalar",
                    "aggregationType": "category",
                    "elements" : [
                        { "name": "elementA1", "type": "int" }
                    ]
                },
                {
                    "name": "entryC",
                    "type": "map",
                    "aggregationType": "xy",
                    "elements" : [
                        { "name": "elementC1", "type": "number" },
                        { "name": "elementC2", "type": "number" }
                    ]
                }
            ]
        }';
        $newP = Product::fromJson($jsonNew);
        $p->update(self::$db, $newP);

        $p = Product::productByName(self::$db, 'org.kde.ProductToUpdate');
        $this->assertJsonStringEqualsJsonString($jsonNew, json_encode($p));
    }

    public function testProductDelete()
    {
        $json = '{
            "name": "org.kde.DeletedProduct",
            "schema": [
                {
                    "name": "entry1",
                    "type": "list",
                    "aggregationType": "numeric",
                    "elements" : [
                        { "name": "element3", "type": "number" }
                    ]
                }
            ]
        }';

        $p = Product::fromJson($json);
        $p->insert(self::$db);
        $p = Product::productByName(self::$db, 'org.kde.DeletedProduct');
        $this->assertJsonStringEqualsJsonString($json, json_encode($p));

        $p->delete(self::$db);
        $p = Product::productByName(self::$db, 'org.kde.DeletedProduct');
        $this->assertNull($p);
    }

    public function testInvalidInput_data()
    {
        return [
            'empty' => [ '{}' ],
            'empty name' => [ '{ "name": "", "schema": [] }' ],
            'invalid name' => [ '{ "name": "123", "schema": [] }' ],
            'non-string name' => [ '{ "name": 123, "schema": [] }' ]
        ];
    }

    /**
     * @dataProvider testInvalidInput_data
     * @expectedException RESTException
     * @exceptedExceptionCode 400
     */
    public function testInvalidInput($json)
    {
        $p = Product::fromJson($json);
    }
}

