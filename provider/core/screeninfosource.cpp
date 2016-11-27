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

#include <QGuiApplication>
#include <QScreen>

using namespace UserFeedback;

QString screenToString(QScreen *screen)
{
    return QStringLiteral("%1x%2 %3dpi").arg(screen->size().width()).arg(screen->size().height()).arg((int)screen->physicalDotsPerInch());
}

ScreenInfoSource::ScreenInfoSource() :
    AbstractDataSource(QStringLiteral("screens"))
{
}

QVariant ScreenInfoSource::data()
{
    QVariantList l;
    foreach (auto screen, QGuiApplication::screens()) {
        QVariantMap m;
        m.insert(QStringLiteral("width"), screen->size().width());
        m.insert(QStringLiteral("height"), screen->size().height());
        l.push_back(m);
    }
    return l;
}
