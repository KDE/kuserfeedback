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

require_once('../../src/server/shared/product.php');
require_once('../../src/server/shared/schemaentry.php');
require_once('../../src/server/shared/schemaentryelement.php');

class SchemaEntryElementTest extends PHPUnit_Framework_TestCase
{
    public function testDataColumnName_data()
    {
        return [
            'normal' => [ 'foo', 'entry_foo' ],
            'dot' => [ 'my.value', 'entry_my_value' ]
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
