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

require_once('../../src/server/shared/aggregation.php');
require_once('../../src/server/shared/datastore.php');

class AggregationTest extends PHPUnit_Extensions_Database_TestCase
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
        $json = '[]';
        $aggrs = Aggregation::fromJson(json_decode($json));
        $this->assertCount(0, $aggrs);

        $json = '[
            { "type": "category", "elements": [ { "type": "value", "schemaEntry": "entry1", "schemaEntryElement": "elem11" } ] },
            { "type": "numeric", "elements": [ { "type": "size", "schemaEntry": "entry2" } ] }
        ]';
        $aggrs = Aggregation::fromJson(json_decode($json));
        $this->assertCount(2, $aggrs);

        $a = $aggrs[0];
        $this->assertInstanceOf(Aggregation::class, $a);
        $this->assertEquals('category', $a->type);
        $this->assertCount(1, $a->elements);
        $a = $aggrs[1];
        $this->assertInstanceOf(Aggregation::class, $a);
        $this->assertEquals('numeric', $a->type);
        $this->assertCount(1, $a->elements);
    }

    public function testLoad()
    {
        $p = Product::productByName(self::$db, 'org.kde.UnitTest');
        $this->assertNotNull($p);

        $aggrs = Aggregation::aggregationsForProduct(self::$db, $p);
        $this->assertCount(2, $aggrs);

        $a = $aggrs[0];
        $this->assertInstanceOf(Aggregation::class, $a);
        $this->assertEquals('category', $a->type);
        $this->assertCount(1, $a->elements);

        $a = $aggrs[1];
        $this->assertInstanceOf(Aggregation::class, $a);
        $this->assertEquals('numeric', $a->type);
        $this->assertCount(1, $a->elements);
    }

    public function testWrite()
    {
        $p = Product::productByName(self::$db, 'org.kde.UnitTest');
        $this->assertNotNull($p);

        $a = new Aggregation;
        $a->type = 'xy';
        $a->elements = json_decode('[
            { "type": "value", "schemaEntry": "entry2", "schemaEntryElement": "element21" },
            { "type": "value", "schemaEntry": "entry2", "schemaEntryElement": "element22" }
        ]');
        Aggregation::update(self::$db, $p, array(0 => $a));

        $aggrs = Aggregation::aggregationsForProduct(self::$db, $p);
        $this->assertCount(1, $aggrs);

        $a = $aggrs[0];
        $this->assertInstanceOf(Aggregation::class, $a);
        $this->assertEquals('xy', $a->type);
        $this->assertCount(2, $a->elements);
    }

    public function testDelete()
    {
        $p = Product::productByName(self::$db, 'org.kde.UnitTest');
        $this->assertNotNull($p);

        $aggrs = Aggregation::aggregationsForProduct(self::$db, $p);
        $this->assertNotEmpty($aggrs);

        Aggregation::delete(self::$db, $p);
        $aggrs = Aggregation::aggregationsForProduct(self::$db, $p);
        $this->assertEmpty($aggrs);
    }

    public function testInvalidJson_data()
    {
        return [
            'nothing' => [ '' ],
            'object' => [ '{}' ],
            'array of non-objects' => [ '[1, 2, 3]' ],
            'missing type' => [ '[{ "elements": [] }]' ]
        ];
    }

    /**
     * @dataProvider testInvalidJson_data
     * @expectedException RESTException
     * @exceptedExceptionCode 400
     */
    public function testInvalidJson($input)
    {
        $aggrs = Aggregation::fromJson(json_decode($input));
    }
}
