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

/** Turns REST requests into method calls. */
class RESTDispatcher
{

public static function dispatch($handler)
{
    $prefix = dirname($_SERVER['PHP_SELF']);
    $command = explode('/', substr($_SERVER['REQUEST_URI'], strlen($prefix) + 1), 2);
    if (sizeof($command) < 1)
        die('Empty REST command.');

    $method = strtolower($_SERVER['REQUEST_METHOD']) . '_' . $command[0];

    if (!method_exists($handler, $method))
        die('Invalid REST command ' . $method . '.');

    if (sizeof($command) == 1)
        $handler->$method();
    else
        $handler->$method($command[1]);
}

}

?>
