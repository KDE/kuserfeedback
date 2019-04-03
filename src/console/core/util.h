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

#ifndef KUSERFEEDBACK_CONSOLE_UTIL_H
#define KUSERFEEDBACK_CONSOLE_UTIL_H

#include <QMetaEnum>
#include <QMetaType>

class QString;

namespace KUserFeedback {
namespace Console {
namespace Util
{
    template<typename Enum, typename Table, std::size_t N>
    Enum stringToEnum(const QString &s, const Table(&lookupTable)[N])
    {
        for (std::size_t i = 0; i < N; ++i) {
            if (s == QLatin1String(lookupTable[i].name))
                return lookupTable[i].type;
        }
        return Enum();
    }

    template<typename Enum>
    QByteArray enumToString(Enum v)
    {
        const auto mo = QMetaType::metaObjectForType(qMetaTypeId<Enum>());
        if (!mo)
            return {};

        const QByteArray typeName = QMetaType::typeName(qMetaTypeId<Enum>());
        const auto idx = typeName.lastIndexOf("::");
        if (idx <= 0)
            return {};

        const auto enumName = typeName.mid(idx + 2);
        const auto enumIdx = mo->indexOfEnumerator(enumName);
        if (enumIdx < 0)
            return {};

        const auto me = mo->enumerator(enumIdx);
        return me.valueToKeys(v);
    }
}
}
}

#endif // KUSERFEEDBACK_CONSOLE_UTIL_H
