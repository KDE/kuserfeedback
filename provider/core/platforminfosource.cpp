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

#include "platforminfosource.h"

#include <QSysInfo>
#include <QVariant>

using namespace UserFeedback;

PlatformInfoSource::PlatformInfoSource() :
    AbstractDataSource(QStringLiteral("platform"))
{
}

QVariant PlatformInfoSource::data()
{
    QVariantMap m;
#if (defined(Q_OS_LINUX) && !defined(Q_OS_ANDROID))
    // on Linux productType() is the distro name
    m.insert(QStringLiteral("os"), QStringLiteral("linux"));
    m.insert(QStringLiteral("version"), QSysInfo::productType() + QLatin1Char('-') + QSysInfo::productVersion());
#else
    m.insert(QStringLiteral("os"), QSysInfo::productType());
    m.insert(QStringLiteral("version"), QSysInfo::productVersion());
#endif
    return m;
}
