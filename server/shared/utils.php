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

class Utils {

private static function normalizeString($input)
{
    $result = '';
    for ($i = 0; $i < strlen($input); $i++) {
        $c = $input[$i];
        if (!ctype_alnum($c))
            $c = '_';
        $result .= $c;
    }
    return $result;
}

public static function tableNameForProduct($productName)
{
    if ($productName == "")
        die("Empty product name.");

    if (!ctype_alpha($productName[0]))
        die("Invalid product name, must start with a letter.");

    $tableName = 'product_' . Utils::normalizeString($productName);
    return $tableName;
}

public static function tableNameForComplexEntry($productName, $entryName)
{
    $productTableName = Utils::tableNameForProduct($productName);
    $productTableName .= '_';
    $productTableName .= Utils::normalizeString($entryName);

    return $productTableName;
}

public static function httpError($responseCode, $message)
{
    http_response_code($responseCode);
    header('Content-Type: text/plain');
    print($message);
    exit(1);
}

}

?>
