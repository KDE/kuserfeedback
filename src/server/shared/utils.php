<?php
/*
    Copyright (C) 2016 Volker Krause <vkrause@kde.org>

    Permission is hereby granted, free of charge, to any person obtaining
    a copy of this software and associated documentation files (the
    "Software"), to deal in the Software without restriction, including
    without limitation the rights to use, copy, modify, merge, publish,
    distribute, sublicense, and/or sell copies of the Software, and to
    permit persons to whom the Software is furnished to do so, subject to
    the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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
