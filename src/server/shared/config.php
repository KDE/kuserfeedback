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

/** Database configuration. */
class Config
{

private $type = null;
private $name = null;
private $host = null;
private $port = null;
private $username = null;
private $password = null;

public function __construct()
{
    // allow test code to override this without requireing a localconfig.php
    global $USERFEEDBACK_DB_DRIVER;
    global $USERFEEDBACK_DB_NAME;

    if (file_exists(__DIR__ . '/../config/localconfig.php'))
        include_once(__DIR__ . '/../config/localconfig.php');

    if (isset($USERFEEDBACK_DB_DRIVER))
        $this->type = strval($USERFEEDBACK_DB_DRIVER);
    else
        throw new RESTException('Missing database configuration!', 500);

    if (isset($USERFEEDBACK_DB_NAME))
        $this->name = strval($USERFEEDBACK_DB_NAME);
    if (isset($USERFEEDBACK_DB_HOST))
        $this->host = strval($USERFEEDBACK_DB_HOST);
    if (isset($USERFEEDBACK_DB_PORT))
        $this->port = intval($USERFEEDBACK_DB_PORT);
    if (isset($USERFEEDBACK_DB_USERNAME))
        $this->username = strval($USERFEEDBACK_DB_USERNAME);
    if (isset($USERFEEDBACK_DB_PASSWORD))
        $this->password = strval($USERFEEDBACK_DB_PASSWORD);

    if ($this->type == 'sqlite') {
        $dirName = dirname($this->name);
        if (!file_exists($dirName))
            mkdir(dirname($this->name), $mode = 0700, $recursive = true);
    }
}

/** PDO DSN */
public function dsn()
{
    switch ($this->type) {
        case 'sqlite':
            return 'sqlite:' . $this->name;
        case 'mysql':
            $s = 'mysql:';
            // TODO: support unix_socket=
            if ($this->host) $s .= 'host=' . $this->host . ';';
            if ($this->port) $s .= 'port=' . $this->port . ';';
            if (is_null($this->name)) break;
            $s .= 'dbname=' . $this->name;
            return $s;
        case 'pgsql':
            $s = 'pgsql:';
            if ($this->host) $s .= 'host=' . $this->host . ';';
            if ($this->port) $s .= 'port=' . $this->port . ';';
            if (is_null($this->name)) break;
            $s .= 'dbname=' . $this->name;
            return $s;
    }
    throw new RESTException('Invalid database configuration!', 500);
}

/** Database user name. */
public function username()
{
    return $this->username;
}

/** Database password. */
public function password()
{
    return $this->password;
}

}

?>
