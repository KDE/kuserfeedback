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
