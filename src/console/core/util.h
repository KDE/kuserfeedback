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
