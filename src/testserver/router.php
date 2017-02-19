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

/** NOTE: This file is for local autotests only, do not deploy! */

$path = explode('/', $_SERVER['REQUEST_URI']);

switch ($path[1]) {
    case 'analytics':
        $_SERVER['PHP_SELF'] = '/analytics/index.php';
        include '../server/analytics/index.php';
        return;
    case 'receiver':
        $_SERVER['PHP_SELF'] = '/receiver/index.php';
        include '../server/receiver/index.php';
        return;
}

http_response_code(404);
header('Content-Type: text/plain');
print('Invalid request URI: ' . $_SERVER['REQUEST_URI']);

?>
