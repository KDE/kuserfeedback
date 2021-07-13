<?php
/*
    SPDX-FileCopyrightText: 2017 Volker Krause <vkrause@kde.org>

    SPDX-License-Identifier: MIT
*/

require_once('../src/server/shared/datastore.php');

// default database configuration for sqlite, can be overridden by localconfig.php
$USERFEEDBACK_DB_DRIVER = 'sqlite';
$USERFEEDBACK_DB_NAME =  ':memory:';

class AbstractDatastoreTest extends PHPUnit\Framework\TestCase
{
    use PHPUnit\DbUnit\TestCaseTrait;

    static protected $db = null;
    protected $conn = null;

    public function getConnection()
    {
        if (is_null($this->conn)) {
            if (is_null(self::$db)) {
                self::$db = new Datastore();
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
}

?>
