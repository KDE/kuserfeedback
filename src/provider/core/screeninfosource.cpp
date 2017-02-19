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

#include "screeninfosource.h"

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QGuiApplication>
#endif
#include <QScreen>
#include <QVariant>

using namespace UserFeedback;

ScreenInfoSource::ScreenInfoSource() :
    AbstractDataSource(QStringLiteral("screens"))
{
}

QVariant ScreenInfoSource::data()
{
    QVariantList l;
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    foreach (auto screen, QGuiApplication::screens()) {
        QVariantMap m;
        m.insert(QStringLiteral("width"), screen->size().width());
        m.insert(QStringLiteral("height"), screen->size().height());
        m.insert(QStringLiteral("dpi"), qRound(screen->physicalDotsPerInch()));
        l.push_back(m);
    }
#endif
    return l;
}
