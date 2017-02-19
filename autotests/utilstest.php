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

require_once('../src/server/shared/utils.php');

class UtilTest extends PHPUnit_Framework_TestCase
{
    public function testTableName()
    {
        $this->assertEquals(Utils::tableNameForProduct("foo.bar"), "product_foo_bar");
    }

    public function testIsValidIdentifier_data()
    {
        return [
            'empty' => [ '', false ],
            'number' => [ 42, false ],
            'numstring' => [ '42', false ],
            'leading number' => [ '1foo', false ],
            'alpha only' => [ 'foo', true ],
            'leading space' => [ ' foo', false ],
            'middle space' => [ 'f o o', false ],
            'trailing space' => [ 'foo ', false ],
            'valid' => [ 'foo42', true ],
            'leading underscore' => [ '_foo', true ],
            'underscore' => [ 'f_o_o', true ],
            'dots and dashes' => [ 'org.kde.unit-test', true ],
            'control' => [ "fo\no", false ]
        ];
    }

    /** @dataProvider testIsValidIdentifier_data */
    public function testIsValidIdentifier($str, $result)
    {
        $this->assertEquals($result, Utils::isValidIdentifier($str));
    }

    public function testNormalize_data()
    {
        return [
            'empty' => [ '', '' ],
            'normal' => [ 'foo', 'foo' ],
            'dot' => [ 'org.kde.foo', 'org_kde_foo' ]
        ];
    }

    /** @dataProvider testNormalize_data */
    public function testNormalize($input, $output)
    {
        $this->assertEquals($output, Utils::normalizeString($input));
    }
}
