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
require_once('../../server/shared/survey.php');

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

    public function testFromJson()
    {
        $s = Survey::fromJson('{
            "name": "mySurvey",
            "url": "http://survey.example/",
            "active": true,
            "id": 42
        }');
        $this->assertEquals($s->name, 'mySurvey');
        $this->assertEquals($s->url, 'http://survey.example/');
        $this->assertEquals($s->active, true);
        $this->assertEquals($s->id, 42);
    }

    public function testToJson()
    {
        $s = new Survey();
        $s->name = 'mySurvey';
        $s->url = 'http://survey.example/';
        $s->active = true;
        $s->id = 42;

        $this->assertJsonStringEqualsJsonString($s->toJson(), '{
            "name": "mySurvey",
            "url": "http://survey.example/",
            "active": true,
            "id": 42
        }');
    }

    public function testGetSurveys()
    {
        $surveys = Survey::surveysForProduct(self::$db, 'I don\'t exist');
        $this->assertCount(0, $surveys);

        $surveys = Survey::surveysForProduct(self::$db, 'org.kde.UnitTest');
        $this->assertCount(2, $surveys);

        $survey1 = $surveys[0];
        $this->assertInstanceOf(Survey::class, $survey1);
        $survey2 = $surveys[1];
        $this->assertInstanceOf(Survey::class, $survey2);

        if ($survey1->name > $survey2->name) {
            $survey1 = $surveys->schema[1];
            $survey2 = $surveys->schema[0];
        }

        $this->assertEquals($survey1->name, 'myActiveSurvey');
        $this->assertEquals($survey1->url, 'http://survey.example/active');
        $this->assertEquals($survey1->active, true);

        $this->assertEquals($survey2->name, 'myInactiveSurvey');
        $this->assertEquals($survey2->url, 'http://survey.example/inactive');
        $this->assertEquals($survey2->active, false);

        $surveys = Survey::activeSurveysForProduct(self::$db, 'org.kde.UnitTest');
        $this->assertCount(1, $surveys);

        $s = $surveys[0];
        $this->assertInstanceOf(Survey::class, $s);
        $this->assertEquals($s->name, 'myActiveSurvey');
        $this->assertEquals($s->url, 'http://survey.example/active');
        $this->assertEquals($s->active, true);
    }

    public function testSurveyInsert()
    {
        $json = '{
            "name": "newSurvey",
            "url": "http://survey.example/new",
            "active": true
        }';

        $p = Product::productByName(self::$db, 'org.kde.UnitTest');
        $this->assertNotNull($p);

        $s = Survey::fromJson($json);
        $s->insert(self::$db, $p);
        $this->assertGreaterThan(0, $s->id);

        $surveys = Survey::activeSurveysForProduct(self::$db, 'org.kde.UnitTest');
        $s = null;
        foreach ($surveys as $survey) {
            if ($survey->name == 'newSurvey') {
                $s = $survey;
                break;
            }
        }
        $this->assertNotNull($s);
        $this->assertEquals($s->name, 'newSurvey');
        $this->assertEquals($s->url, 'http://survey.example/new');
        $this->assertEquals($s->active, true);
    }

    public function testSurveyUpdate()
    {
        $json = '{
            "name": "myChangedSurvey",
            "url": "http://survey.example/changed",
            "active": false,
            "id": 101
        }';

        $s = Survey::fromJson($json);
        $s->update(self::$db);

        $surveys = Survey::surveysForProduct(self::$db, 'org.kde.UnitTest');
        $s = null;
        foreach ($surveys as $survey) {
            if ($survey->name == 'myChangedSurvey') {
                $s = $survey;
                break;
            }
        }
        $this->assertNotNull($s);
        $this->assertEquals($s->name, 'myChangedSurvey');
        $this->assertEquals($s->url, 'http://survey.example/changed');
        $this->assertEquals($s->active, false);
    }

    public function testSurveyDelete()
    {
        $json = '{
            "name": "myInactiveSurvey",
            "url": "http://survey.example/inactive",
            "active": false,
            "id": 102
        }';

        $s = Survey::fromJson($json);
        $s->delete(self::$db);

        $surveys = Survey::surveysForProduct(self::$db, 'org.kde.UnitTest');
        $s = null;
        foreach ($surveys as $survey) {
            if ($survey->name == 'myInactiveSurvey') {
                $s = $survey;
                break;
            }
        }
        $this->assertNull($s);
    }

    public function testInvalidInput_data()
    {
        return [
            'empty' => [ '{}' ],
            'mising url' => [ '{ "name": "surveyName" }' ],
            'missing active' => [ '{ "name": "surveyName", "url": "http://survey.example/" }' ],
            'empty name' => [ '{ "name": "", "url": "http://survey.example/", "active": false }' ],
            'non-string name' => [ '{ "name": 123, "url": "http://survey.example/", "active": false }' ],
            'non-numberic id' => [ '{ "name": "myName", "url": "http://survey.example/", "active": true, "id": "string" }' ]
        ];
    }

    /**
     * @dataProvider testInvalidInput_data
     * @expectedException RESTException
     * @exceptedExceptionCode 400
     */
    public function testInvalidInput($json)
    {
        $s = Survey::fromJson($json);
    }
}


