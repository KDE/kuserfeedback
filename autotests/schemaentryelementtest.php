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
require_once('../src/server/shared/schemaentryelement.php');

class SchemaEntryElementTest extends PHPUnit\Framework\TestCase
{
    public function testDataColumnName_data()
    {
        return [
            'normal' => [ 'foo', 'col_data_entry_foo' ],
            'dot' => [ 'my.value', 'col_data_entry_my_value' ]
        ];
    }

    /** @dataProvider testDataColumnName_data */
    public function testDataTableName($input, $output)
    {
        $p = new Product;
        $p->name = 'org.kde.TestProduct';
        $se = new SchemaEntry($p);
        $se->name = 'entry';
        $see = new SchemaEntryElement($se);
        $see->name = $input;
        $this->assertEquals($output, $see->dataColumnName());
    }

    public function testInvalidJson_data()
    {
        return [
            'empty' => [ '{}' ],
            'empty name' => [ '{ "name": "", "type": "string" }' ],
            'empty type' => [ '{ "name": "foo", "type": "" }' ],
            'invalid type' => [ '{ "name": "foo", "type": "bla" }' ],
            'invalid name' => [ '{ "name": " foo ", "type": "string" }' ],
            'invalid name 2' => [ '{ "name": "1foo ", "type": "string" }' ]
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
        $se = new SchemaEntry($p);
        SchemaEntryElement::fromJson(json_decode('[ ' . $input . ' ]'), $se);
    }
}
