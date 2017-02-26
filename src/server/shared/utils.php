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

class Utils {

public static function isValidIdentifier($str)
{
    if (!is_string($str) || strlen($str) <= 0)
        return false;
    if (!ctype_alpha($str[0]) && $str[0] != '_') // must start with a letter or underscore
        return false;
    for ($i = 0; $i < strlen($str); $i++) {
        if (ctype_space($str[$i]) || ctype_cntrl($str[$i]))
            return false;
    }
    return true;
}

public static function normalizeString($input)
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

/** Driver-dependen default SQL string type. */
public static function sqlStringType($driver)
{
    switch ($driver) {
        case 'sqlite':
        case 'pgsql':
            return 'VARCHAR';
        case 'mysql':
            return 'VARCHAR(255)';
    }
    throw new RESTException('Unsupported database driver.', 500);
}

/** Driver-dependent column declaration for a numeric auto-increment primary key. */
public static function primaryKeyColumnDeclaration($driver, $columnName)
{
    switch ($driver) {
        case 'sqlite':
            return $columnName . ' INTEGER PRIMARY KEY AUTOINCREMENT';
        case 'mysql':
            return $columnName . ' INTEGER PRIMARY KEY AUTO_INCREMENT';
        case 'pgsql':
            return $columnName . ' SERIAL PRIMARY KEY';
    }
    throw new RESTException('Unsupported database driver.', 500);
}

public static function httpError($responseCode, $message)
{
    error_log('UserFeedback ERROR: ' . $message . ' (' . $responseCode . ')');
    http_response_code($responseCode);
    header('Content-Type: text/plain');
    print($message);
    exit(1);
}

}

?>
