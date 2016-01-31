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

public static function tableNameForProduct($productName)
{
    if ($productName == "")
        die("Empty product name.");

    if (!ctype_alpha($productName[0]))
        die("Invalid product name, must start with a letter.");

    $tableName = "product_";
    for ($i = 0; $i < strlen($productName); $i++) {
        $c = $productName[$i];
        if (!ctype_alnum($c))
            $c = '_';
        $tableName .= $c;
    }

    return $tableName;
}

}

?>
