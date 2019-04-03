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

require_once('abstractdatastoretest.php');
require_once('../src/server/shared/aggregation.php');

class AggregationTest extends AbstractDatastoreTest
{
    public function testFromJson()
    {
        $json = '[]';
        $aggrs = Aggregation::fromJson(json_decode($json));
        $this->assertCount(0, $aggrs);

        $json = '[
            { "type": "category", "name": "category elem11", "elements": [ { "type": "value", "schemaEntry": "entry1", "schemaEntryElement": "elem11" } ] },
            { "type": "numeric", "name": "size: entry2", "elements": [ { "type": "size", "schemaEntry": "entry2" } ] }
        ]';
        $aggrs = Aggregation::fromJson(json_decode($json));
        $this->assertCount(2, $aggrs);

        $a = $aggrs[0];
        $this->assertInstanceOf(Aggregation::class, $a);
        $this->assertEquals('category', $a->type);
        $this->assertEquals('category elem11', $a->name);
        $this->assertCount(1, $a->elements);
        $a = $aggrs[1];
        $this->assertInstanceOf(Aggregation::class, $a);
        $this->assertEquals('numeric', $a->type);
        $this->assertEquals('size: entry2', $a->name);
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
        $a->name = 'n1';
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
        $this->assertEquals('n1', $a->name);
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
            'missing type' => [ '[{ "elements": [] }]' ],
            'missing name' => [ '[{ "type": "category", "elements": [] }]' ]
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
