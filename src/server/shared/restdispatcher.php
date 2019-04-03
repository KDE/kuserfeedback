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
require_once('utils.php');

/** Turns REST requests into method calls. */
class RESTDispatcher
{

public static function dispatch($handler)
{
    $prefix = dirname($_SERVER['PHP_SELF']);
    $command = explode('/', substr($_SERVER['REQUEST_URI'], strlen($prefix) + 1), 3);

    try {
        if (sizeof($command) < 1)
            Utils::httpError(400, 'Empty REST command.');

        $method = strtolower($_SERVER['REQUEST_METHOD']) . '_' . $command[0];

        if (!method_exists($handler, $method))
            Utils::httpError(400, 'Invalid REST command ' . $method . '.');

        for ($i = 1; $i < count($command); $i++) {
            if (is_string($command[$i]))
                $command[$i] = urldecode($command[$i]);
        }

        switch(sizeof($command)) {
            case 1:
                $handler->$method();
                break;
            case 2:
                $handler->$method($command[1]);
                break;
            case 3:
                $handler->$method($command[1], $command[2]);
                break;
        }
    } catch (RESTException $e) {
        Utils::httpError($e->getCode(), $e->getMessage());
    }
}

}

?>
