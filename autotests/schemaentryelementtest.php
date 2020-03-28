<?php
/*
    SPDX-FileCopyrightText: 2016 Volker Krause <vkrause@kde.org>

    SPDX-License-Identifier: MIT
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
