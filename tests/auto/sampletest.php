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
require_once('../../server/shared/sample.php');

class SurveyTest extends PHPUnit_Extensions_Database_TestCase
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

    public function testSampleInsert()
    {
        $p = Product::productByName(self::$db, 'org.kde.UnitTest');
        $this->assertNotNull($p);
        $p->name = 'org.kde.MyProduct';
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
        // TODO test insertion of map entries
    }
}



