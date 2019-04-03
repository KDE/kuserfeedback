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

/** NOTE: This file is for local autotests only, do not deploy! */

// default database configuration for sqlite, can be overriden by localconfig.php
$USERFEEDBACK_DB_DRIVER = 'sqlite';
$USERFEEDBACK_DB_NAME =  __DIR__ . '/../server/data/db.sqlite';

$path = explode('/', $_SERVER['REQUEST_URI']);

switch ($path[1]) {
    case 'admin':
        $_SERVER['PHP_SELF'] = '/admin/index.php';
        include '../server/admin/index.php';
        return;
    case 'analytics':
        $_SERVER['PHP_SELF'] = '/analytics/index.php';
        include '../server/analytics/index.php';
        return;
    case 'receiver':
        $_SERVER['PHP_SELF'] = '/receiver/index.php';
        include '../server/receiver/index.php';
        return;

    // redirection test cases
    case 'absRedirect':
        http_response_code(307);
        $url = str_replace('/absRedirect/', '/', $_SERVER['REQUEST_URI']);
        header('Location: http://'. $_SERVER['SERVER_NAME'] . ':' . $_SERVER['SERVER_PORT'] . $url);
        return;
    case 'relRedirect':
        http_response_code(307);
        $url = str_replace('/relRedirect/', '/../', $_SERVER['REQUEST_URI']);
        header('Location: ' . $url);
        return;
    case 'circleRedirect':
        http_response_code(307);
        header('Location: http://'. $_SERVER['SERVER_NAME'] . ':' . $_SERVER['SERVER_PORT'] . $_SERVER['REQUEST_URI']);
        return;
}

http_response_code(404);
header('Content-Type: text/plain');
print('Invalid request URI: ' . $_SERVER['REQUEST_URI']);

?>
