#!/bin/bash

find "$@" -name '*.h' -o -name '*.cpp' -o -name '*.qml' -o -name '*.c' -o -name "*.qdoc" | grep -v /3rdparty/ | grep -v /build | while read FILE; do
    if grep -qiE "Copyright \(C\) [0-9, -]{4,} " "$FILE" ; then continue; fi
    thisfile=`basename $FILE`
    authorName=`git config user.name`
    authorEmail=`git config user.email`
    thisYear=`date +%Y`
    cat <<EOF > "$FILE".tmp
/*
    Copyright (C) $thisYear $authorName <$authorEmail>

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

EOF
    cat "$FILE" >> "$FILE".tmp
    mv "$FILE".tmp "$FILE"
done

