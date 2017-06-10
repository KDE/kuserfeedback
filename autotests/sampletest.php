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

require_once('abstractdatastoretest.php');
require_once('../src/server/shared/product.php');
require_once('../src/server/shared/sample.php');

class SurveyTest extends AbstractDatastoreTest
{
    public function testListSampleInsert()
    {
        $p = Product::productByName(self::$db, 'org.kde.UnitTest');
        $this->assertNotNull($p);
        $p->name = 'org.kde.MyListProduct';
        $p->insert(self::$db); // HACK create a new product, so the data tables are created correctly

        $sample = '{
            "entry1": {
                "element11": "aString",
                "element12": true
            },
            "entry2": [
                { "element21": 14, "element22": 1.5 },
                { "element21": 16, "element22": 1.7 }
            ]
        }';

        Sample::insert(self::$db, $sample, $p);
        Sample::insert(self::$db, $sample, $p);

        $dataJson = Sample::dataAsJson(self::$db, $p);
        $data = json_decode($dataJson);
        $this->assertTrue(is_array($data));
        $this->assertCount(2, $data);
        $d0 = $data[0];
        $this->assertObjectHasAttribute('timestamp', $d0);
        $this->assertObjectHasAttribute('entry1', $d0);
        $this->assertObjectHasAttribute('entry2', $d0);
        $d01 = $d0->{'entry2'};
        $this->assertCount(2, $d01);
    }

    public function testMapSampleInsert()
    {
        $p = Product::productByName(self::$db, 'org.kde.UnitTest');
        $this->assertNotNull($p);
        $p->name = 'org.kde.MyMapProduct';
        $p->schema[1]->type = SchemaEntry::MAP_TYPE;
        $p->insert(self::$db);

        $sample = '{
            "entry1": {
                "element11": "aString",
                "element12": true
            },
            "entry2": {
                "key1": { "element21": 14, "element22": 1.5 },
                "key2": { "element21": 16, "element22": 1.7 }
            }
        }';

        Sample::insert(self::$db, $sample, $p);
        $dataJson = Sample::dataAsJson(self::$db, $p);
        $data = json_decode($dataJson);
        $this->assertTrue(is_array($data));
        $this->assertCount(1, $data);
        $d0 = $data[0];
        $this->assertObjectHasAttribute('timestamp', $d0);
        $this->assertObjectHasAttribute('entry1', $d0);
        $this->assertObjectHasAttribute('entry2', $d0);
        $d01 = $d0->{'entry2'};
        $this->assertObjectHasAttribute('key1', $d01);
        $this->assertObjectHasAttribute('key2', $d01);
    }

    public function testEmptyInsert()
    {
        $p = Product::productByName(self::$db, 'org.kde.UnitTest');
        $this->assertNotNull($p);
        $p->name = 'org.kde.MyEmptyProduct';
        $p->insert(self::$db);

        $sample = '{
            "someRandomStuff": "not part of the schema",
            "someOtherStuff": 42
        }';

        Sample::insert(self::$db, $sample, $p);
        $dataJson = Sample::dataAsJson(self::$db, $p);
        $data = json_decode($dataJson);
        $this->assertTrue(is_array($data));
        $this->assertCount(1, $data);
        $d0 = $data[0];
        $this->assertObjectHasAttribute('timestamp', $d0);
        $this->assertObjectNotHasAttribute('entry1', $d0);
        $this->assertObjectNotHasAttribute('someRandomStuff', $d0);
    }

    public function testInvalidInsert_data()
    {
        return [
            'empty' => [ '' ],
            'array' => [ '[]' ],
            'missing id' => [ '{ "timestamp": "2016-12-18 12:42:35" }' ],
            'missing timestamp' => [ '{ "id": 42 }' ]
        ];
    }

    /**
     * @dataProvider testInvalidInsert_data
     * @expectedException RESTException
     * @exceptedExceptionCode 400
     */
    public function testInvalidInsert($input)
    {
        $p = Product::productByName(self::$db, 'org.kde.UnitTest');
        $this->assertNotNull($p);

        Sample::insert(self::$db, $input, $p);
    }

    public function testImport()
    {
        $p = Product::productByName(self::$db, 'org.kde.UnitTest');
        $this->assertNotNull($p);
        $p->name = 'org.kde.MyCleanProduct';
        $p->insert(self::$db);

        $input = '[{
            "timestamp": "2016-12-18 12:42:35",
            "entry1": { "element11": "firstString" },
            "entry2": [ { "element21": 12 } ]
        }, {
            "timestamp": "2016-12-19 15:12:10",
            "entry1": { "element12": true },
            "entry2": [ { "element22": 1.3 } ]
        }]';
        Sample::import(self::$db, $input, $p);

        $jsonOutput = json_decode(Sample::dataAsJson(self::$db, $p), true);
        unset($jsonOutput[0]['id']);
        unset($jsonOutput[1]['id']);
        $this->assertJsonStringEqualsJsonString($input, json_encode($jsonOutput));
    }

    public function testInvalidImport_data()
    {
        return [
            'nothing' => [ '' ],
            'empty' => [ '{}' ],
            'object' => [ '{ "id": 42, "timestamp": "2016-12-18 12:42:35" }' ],
            'missing timestamp' => [ '[{ "id": 42 }]' ],
            'id present' => [ '[{ "id": 42, "timestamp": "2016-12-18 12:42:35" }]' ]
        ];
    }

    /**
     * @dataProvider testInvalidImport_data
     * @expectedException RESTException
     * @exceptedExceptionCode 400
     */
    public function testInvalidImport($input)
    {
        $p = Product::productByName(self::$db, 'org.kde.UnitTest');
        $this->assertNotNull($p);

        Sample::import(self::$db, $input, $p);
    }
}
