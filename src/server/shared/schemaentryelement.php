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

require_once('restexception.php');
require_once('utils.php');

/** Represents a product schema entry element. */
class SchemaEntryElement
{
    public $name;
    public $type;

    private $schemaEntry = null;

    const STRING_TYPE = 'string';
    const INT_TYPE = 'int';
    const NUMBER_TYPE = 'number';
    const BOOL_TYPE = 'bool';

    public function __construct(SchemaEntry $entry)
    {
        $this->schemaEntry = &$entry;
    }

    /** Checks if this schema entry element is valid. */
    public function isValid()
    {
        if ($this->type != self::STRING_TYPE && $this->type != self::INT_TYPE && $this->type != self::NUMBER_TYPE && $this->type != self::BOOL_TYPE)
            return false;
        if (!Utils::isValidIdentifier($this->name))
            return false;
        return true;
    }

    /** Column name in the data table. */
    public function dataColumnName()
    {
        $colName = 'col_data_' . Utils::normalizeString($this->schemaEntry->name) . '_' . Utils::normalizeString($this->name);
        return strtolower($colName);
    }

    /** Insert this element into storage. */
    public function insert(Datastore $db, $entryId)
    {
        $stmt = $db->prepare('INSERT INTO
            tbl_schema_element (col_schema_id, col_name, col_type)
            VALUES (:schemaId, :name, :type)
        ');
        $stmt->bindValue(':schemaId', $entryId, PDO::PARAM_INT);
        $stmt->bindValue(':name', $this->name, PDO::PARAM_STR);
        $stmt->bindValue(':type', $this->type, PDO::PARAM_STR);
        $db->execute($stmt);

        if ($this->schemaEntry->isScalar())
            $this->createScalarDataTableColumn($db);
        else
            $this->createNonScalarDataTableColumn($db);
    }

    /** Delete this element from storage. */
    public function delete(Datastore $db, $entryId)
    {
        $stmt = $db->prepare('
            DELETE FROM tbl_schema_element
            WHERE col_schema_id = :schemaId AND col_name = :name
        ');
        $stmt->bindValue(':schemaId', $entryId, PDO::PARAM_INT);
        $stmt->bindValue(':name', $this->name, PDO::PARAM_STR);
        $db->execute($stmt);
    }

    /** Drop data table column for this element. */
    public function dropDataColumn(Datastore $db)
    {
        if ($db->driver() == 'sqlite') {
            error_log('Sqlite does not support dropping columns.');
            return;
        }

        if ($this->schemaEntry->isScalar()) {
            $stmt = $db->prepare('ALTER TABLE ' . $this->schemaEntry->product()->dataTableName()
                . ' DROP COLUMN ' . $this->dataColumnName());
            $db->execute($stmt);
        } else {
            $stmt = $db->prepare('ALTER TABLE ' . $this->schemaEntry->dataTableName()
                . ' DROP COLUMN ' . $this->dataColumnName());
            $db->execute($stmt);
        }
    }

    /** Convert a JSON array into an array of SchemaEntryElement instances. */
    static public function fromJson($jsonArray, SchemaEntry &$entry)
    {
        $elems = array();
        foreach ($jsonArray as $jsonObj) {
            if (!property_exists($jsonObj, 'name') || !property_exists($jsonObj, 'type'))
                throw new RESTException('Incomplete schema entry element.', 400);
            $e = new SchemaEntryElement($entry);
            $e->name = $jsonObj->name;
            $e->type = $jsonObj->type;
            if (!$e->isValid())
                throw new RESTException('Invalid schema entry element.', 400);
            array_push($elems, $e);
        }
        return $elems;
    }


    /** SQL type for this element. */
    private function sqlType(Datastore $db)
    {
        switch ($this->type) {
            case self::STRING_TYPE: return Utils::sqlStringType($db->driver());
            case self::INT_TYPE: return 'INTEGER';
            case self::NUMBER_TYPE: return 'REAL';
            case self::BOOL_TYPE: return 'BOOLEAN';
        }
        throw new RESTException('Unsupported schema entry element type.', 400);
    }

    /** Creates a data table entry for scalar elements. */
    private function createScalarDataTableColumn(Datastore $db)
    {
        $stmt = $db->prepare('ALTER TABLE ' . $this->schemaEntry->product()->dataTableName()
            . ' ADD COLUMN ' . $this->dataColumnName() . ' ' . $this->sqlType($db));
        $db->execute($stmt);
    }

    /** Creates a data table entry for non-scalar elements. */
    private function createNonScalarDataTableColumn(Datastore $db)
    {
        $stmt = $db->prepare('ALTER TABLE ' . $this->schemaEntry->dataTableName()
            . ' ADD COLUMN ' . $this->dataColumnName() . ' ' . $this->sqlType($db));
        $db->execute($stmt);
    }
}

?>
