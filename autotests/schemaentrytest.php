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

require_once('../src/server/shared/product.php');
require_once('../src/server/shared/schemaentry.php');

class SchemaEntryTest extends PHPUnit\Framework\TestCase
{
    public function testDataTableName_data()
    {
        return [
            'normal' => [ 'foo', 'pd2_org_kde_testproduct__foo' ],
            'dot' => [ 'my.value', 'pd2_org_kde_testproduct__my_value' ],
            'uppercase' => [ 'UPPER', 'pd2_org_kde_testproduct__upper' ]
        ];
    }

    /** @dataProvider testDataTableName_data */
    public function testDataTableName($input, $output)
    {
        $p = new Product;
        $p->name = 'org.kde.TestProduct';
        $se = new SchemaEntry($p);
        $se->name = $input;
        $this->assertEquals($output, $se->dataTableName());
    }

    public function testInvalidJson_data()
    {
        return [
            'empty' => [ '{}' ],
            'empty name' => [ '{ "name": "", "type": "scalar", "aggregationType": "none" }' ],
            'empty type' => [ '{ "name": "foo", "type": "", "aggregationType": "none" }' ],
            'invalid type' => [ '{ "name": "foo", "type": "bla", "aggregationType": "none" }' ],
            'invalid name' => [ '{ "name": " foo ", "type": "scalar", "aggregationType": "none" }' ],
            'invalid name 2' => [ '{ "name": "1foo ", "type": "scalar", "aggregationType": "none" }' ]
        ];
    }

    /**
     * @dataProvider testInvalidJson_data
     * @expectedException RESTException
     * @exceptedExceptionCode 400
     */
    public function testInvalidJson($input)
    {
        $p = new Product;
        $se = SchemaEntry::fromJson(json_decode('[ ' . $input . ' ]'), $p);
    }
}

