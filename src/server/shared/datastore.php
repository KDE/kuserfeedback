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

require_once( __DIR__ . '/../config/config.php');
require_once('restexception.php');
require_once('utils.php');

/** Handles all database operations. */
class DataStore {

private $db;

function __construct($dsn = NULL)
{
    try {
        if (is_null($dsn))
            $this->db = new PDO(Config::dsn());
        else
            $this->db = new PDO($dsn);
    } catch (PDOException $e) {
        die('Database connection failed: ' . $e->getMessage());
    }
}

/** Access to the PDO handle. */
public function pdoHandle()
{
    return $this->db;
}

/** Prepare a query. */
public function prepare($queryString)
{
    try {
        $res = $this->db->prepare($queryString);
        $this->checkError($res);
    } catch (PDOException $e) {
        throw new RESTException($e->getMessage(), 500);
    }
    return $res;
}

/** Execute a prepared query. */
public function execute(PDOStatement $stmt, $bindValues = array())
{
    try {
        $stmt->execute($bindValues);
        $this->checkError($stmt);
    } catch (PDOException $e) {
        throw new RESTException($e->getMessage(), 500);
    }
}

/** Check database result for errors, and if so, bail out. */
private function checkError($res)
{
    if ($res === FALSE) {
        $err = $this->db->errorInfo();
        $msg = "SQLSTATE: " . $err[0] . "\nDriver error code: " . $err[1] . "\nDriver error message: " . $err[2];
        throw new RESTException($msg, 500);
    }
}

/** Begin transaction. */
public function beginTransaction()
{
    $this->db->beginTransaction();
}

/** Commit transaction. */
public function commit()
{
    $this->db->commit();
}

/** Verify the database schema, and fix if needed. */
public function checkSchema()
{
    $currentVersion = $this->schemaVersion();
    $schemaFile = __DIR__ . '/schema.json';
    $schemaDefs = json_decode(file_get_contents($schemaFile), true);
    $targetVersion = count($schemaDefs['schema']);

    if ($currentVersion == $targetVersion) {
        echo('Schema is up-to-date.');
        return;
    }

    if ($currentVersion > $targetVersion || $currentVersion < 0)
        die('Current schema version is invalid: ' . $currentVersion . '.');

    # apply database updates
    echo('Current schema version: ' . $currentVersion . ' should be: ' . $targetVersion);
    for ($i = $currentVersion; $i < $targetVersion; $i++) {
        echo("Applying update $i...");
        $this->applySchemaChange($schemaDefs['schema'][$i]);
    }
}

/** Returns the current schema version. */
private function schemaVersion()
{
    $res = $this->db->query('SELECT version FROM schema_version');
    if ($res === FALSE)
        return 0;
    foreach ($res as $row)
        return $row['version'];
    return 0;
}

/** Applies a list of schema setup commands. */
private function applySchemaChange($schemaDef)
{
    foreach($schemaDef['sql'] as $cmd) {
        $res = $this->db->exec($cmd);
        $this->checkError($res);
    }
    $res = $this->db->exec('UPDATE schema_version SET version = ' . $schemaDef['version']);
    $this->checkError($res);
}

}

?>
