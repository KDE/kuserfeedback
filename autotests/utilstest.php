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

require_once('../src/server/shared/utils.php');

class UtilTest extends PHPUnit\Framework\TestCase
{
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

    public function testPrimaryKeyColumn_data()
    {
        return [
            'sqlite' => [ 'sqlite', 'id', 'id INTEGER PRIMARY KEY AUTOINCREMENT' ],
            'pgsql' => [ 'pgsql', 'id', 'id SERIAL PRIMARY KEY' ],
            'mysql' => [ 'mysql', 'id', 'id INTEGER PRIMARY KEY AUTO_INCREMENT' ]
        ];
    }

    /** @dataProvider testPrimaryKeyColumn_data */
    public function testPrimaryKeyColumn($driver, $name, $output)
    {
        $this->assertEquals($output, Utils::primaryKeyColumnDeclaration($driver, $name));
    }

    public function testSqlStringType_data()
    {
        return [
            'sqlite' => [ 'sqlite', 'VARCHAR' ],
            'mysql' => [ 'mysql', 'VARCHAR(255)' ],
            'pgsql' => [ 'pgsql', 'VARCHAR' ],
        ];
    }

    /** @dataProvider testSqlStringType_data */
    public function testSqlStringType($driver, $output)
    {
        $this->assertEquals($output, Utils::sqlStringType($driver));
    }
}
