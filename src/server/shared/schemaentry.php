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

require_once('schemaentryelement.php');
require_once('utils.php');

/** Represents a product schema entry. */
class SchemaEntry
{
    public $name;
    public $type;
    public $elements = array();

    private $entryId = -1;
    private $m_product = null;

    const SCALAR_TPYE = 'scalar';
    const LIST_TYPE = 'list';
    const MAP_TYPE = 'map';

    public function __construct(Product $product)
    {
        $this->m_product = &$product;
    }

    /** Checks if this is a schema entry. */
    public function isValid()
    {
        if ($this->type != self::SCALAR_TPYE && $this->type != self::LIST_TYPE && $this->type != self::MAP_TYPE)
            return false;
        if (!Utils::isValidIdentifier($this->name))
            return false;
        return true;
    }

    /** Checks if this is a scalar type, ie. samples go into the primary data table. */
    public function isScalar()
    {
        return $this->type === self::SCALAR_TPYE;
    }

    /** Returns the product this entry belongs to. */
    public function product()
    {
        return $this->m_product;
    }

    /** Load product schema from storage. */
    static public function loadSchema(Datastore $db, Product &$product)
    {
        $stmt = $db->prepare('SELECT
                product_schema.id, product_schema.name, product_schema.type, schema_elements.name, schema_elements.type
            FROM schema_elements JOIN product_schema ON (product_schema.id = schema_elements.schemaid)
            WHERE product_schema.productid = :productId
            ORDER BY product_schema.id
        ');
        $stmt->bindValue(':productId', $product->id(), PDO::PARAM_INT);
        $db->execute($stmt);
        $schema = array();
        $entry = new SchemaEntry($product);
        foreach ($stmt as $row) {
            if ($entry->entryId != $row[0]) {
                if ($entry->isValid()) {
                    array_push($schema, $entry);
                    $entry = new SchemaEntry($product);
                }
                $entry->entryId = $row[0];
                $entry->name = $row[1];
                $entry->type = $row[2];
            }
            $elem = new SchemaEntryElement($entry);
            $elem->name = $row[3];
            $elem->type = $row[4];
            array_push($entry->elements, $elem);
        }
        if ($entry->isValid())
            array_push($schema, $entry);

        return $schema;
    }

    /** Insert a new schema entry into storage. */
    public function insert(Datastore $db, $productId)
    {
        $stmt = $db->prepare('INSERT INTO
            product_schema (productid, name, type)
            VALUES (:productId, :name, :type)
        ');
        $stmt->bindValue(':productId', $productId, PDO::PARAM_INT);
        $stmt->bindValue(':name', $this->name, PDO::PARAM_STR);
        $stmt->bindValue(':type', $this->type, PDO::PARAM_STR);
        $db->execute($stmt);
        $this->entryId = $db->pdoHandle()->lastInsertId();

        // add secondary data tables for non-scalars
        switch ($this->type) {
            case self::LIST_TYPE:
                $this->createListDataTable($db);
                break;
            case self::MAP_TYPE:
                $this->createMapDataTable($db);
                break;
        }

        // add elements (which will create data table columns, so this needs to be last)
        foreach ($this->elements as $elem)
            $elem->insert($db, $this->entryId);
    }

    /** Update this schema entry in storage. */
    public function update(Datastore $db, SchemaEntry $newEntry)
    {
        // TODO reject type changes

        // update elements
        $oldElements = array();
        foreach ($this->elements as $oldElem)
            $oldElements[$oldElem->name] = $oldElem;

        foreach ($newEntry->elements as $newElem) {
            if (array_key_exists($newElem->name, $oldElements)) {
                // update
                // TODO this would require type conversion of the data!?
            } else {
                // insert
                $newElem->insert($db, $this->entryId);
            }
            unset($oldElements[$newElem->name]);
        }

        // delete whatever is left
        // TODO also remove data
        foreach($oldElements as $elem)
            $elem->delete($db, $this->entryId);
    }

    /** Delete this schema entry from storage. */
    public function delete(Datastore $db, $productId)
    {
        // TODO drop primary data table columns
        // drop secondary data tables
        if (!$this->isScalar())
            $this->dropDataTable($db);

        // delete elements
        $stmt = $db->prepare('DELETE FROM schema_elements WHERE schemaid = :id');
        $stmt->bindValue(':id', $this->entryId, PDO::PARAM_INT);
        $db->execute($stmt);

        // delete entry
        $stmt = $db->prepare('DELETE FROM product_schema WHERE id = :id');
        $stmt->bindValue(':id', $this->entryId, PDO::PARAM_INT);
        $db->execute($stmt);
    }

    /** Convert a JSON object into an array of SchemaEntry instances. */
    static public function fromJson($jsonArray, Product &$product)
    {
        $entries = array();
        foreach ($jsonArray as $jsonObj) {
            if (!property_exists($jsonObj, 'name') || !property_exists($jsonObj, 'type'))
                throw new RESTException('Incomplete schema entry object.', 400);
            $e = new SchemaEntry($product);
            $e->name = strval($jsonObj->name);
            $e->type = strval($jsonObj->type);
            if (property_exists($jsonObj, 'elements'))
                $e->elements = SchemaEntryElement::fromJson($jsonObj->elements, $e);
            if (!$e->isValid())
                throw new RESTException('Invalid schema entry.', 400);
            array_push($entries, $e);
        }
        return $entries;
    }

    /** Data table name for secondary data tables. */
    public function dataTableName()
    {
        $tableName = $this->product()->dataTableName() . '_' . Utils::normalizeString($this->name);
        return strtolower($tableName);
    }


    /** Create secondary data tables for list types. */
    private function createListDataTable(Datastore $db)
    {
        $stmt = $db->prepare('CREATE TABLE ' . $this->dataTableName(). ' ('
            . Utils::primaryKeyColumnDeclaration($db->driver(), 'id') . ', '
            . 'sampleid INTEGER REFERENCES ' . $this->product()->dataTableName() . '(id))'
        );
        $db->execute($stmt);
    }

    /** Create secondary data tables for map types. */
    private function createMapDataTable(Datastore $db)
    {
        $stmt = $db->prepare('CREATE TABLE ' . $this->dataTableName(). ' ('
            . Utils::primaryKeyColumnDeclaration($db->driver(), 'id') . ', '
            . 'sampleid INTEGER REFERENCES ' . $this->product()->dataTableName() . '(id), '
            . 'mapkey ' . Utils::sqlStringType($db->driver()) . ' NOT NULL)'
        );
        $db->execute($stmt);
    }

    /** Drop secondary data tables for non-scalar types. */
    private function dropDataTable(Datastore $db)
    {
        $stmt = $db->prepare('DROP TABLE ' . $this->dataTableName());
        $db->execute($stmt);
    }
}

?>
