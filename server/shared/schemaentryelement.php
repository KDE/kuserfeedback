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

    /** Insert this element into storage. */
    public function insert(Datastore $db, $entryId)
    {
        $stmt = $db->prepare('INSERT INTO
            schema_elements (schemaId, name, type)
            VALUES (:schemaId, :name, :type)
        ');
        $db->execute($stmt, array(
            ':schemaId' => $entryId,
            ':name' => $this->name,
            ':type' => $this->type
        ));

        if (!$this->schemaEntry->isScalar())
            $this->createNonScalarDataTableColor($db);
    }

    /** Delete this element from storage. */
    public function delete(Datastore $db, $entryId)
    {
        $stmt = $db->prepare('
            DELETE FROM schema_elements
            WHERE schemaId = :schemaId AND name = :name
        ');
        $db->execute($stmt, array(
            ':schemaId' => $entryId,
            ':name' => $this->name
        ));
    }

    /** Convert a JSON array into an array of SchemaEntryElement instances. */
    static public function fromJson($jsonArray, SchemaEntry &$entry)
    {
        $elems = array();
        foreach ($jsonArray as $jsonObj) {
            $e = new SchemaEntryElement($entry);
            $e->name = $jsonObj->name;
            $e->type = $jsonObj->type;
            array_push($elems, $e);
        }
        return $elems;
    }


    /** SQL type for this element. */
    private function sqlType()
    {
        switch ($this->type) {
            case self::STRING_TYPE: return 'VARCHAR';
            case self::INT_TYPE: return 'INTEGER';
            case self::NUMBER_TYPE: return 'REAL';
            case self::BOOL_TYPE: return 'BOOLEAN';
        }
        throw new RESTException('Unsupported schema entry element type.', 400);
    }

    /** Creates a data table entry for non-scalar types. */
    private function createNonScalarDataTableColor(Datastore $db)
    {
        $stmt = $db->prepare('ALTER TABLE ' . $this->schemaEntry->dataTableName()
            . ' ADD COLUMN ' . $this->name . ' ' . $this->sqlType());
        $db->execute($stmt);
    }
}

?>
