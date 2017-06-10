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

#include "schemaentrytemplates.h"

#include <core/product.h>

#include <QDebug>
#include <QDirIterator>
#include <QJsonArray>
#include <QJsonDocument>
#include <QStandardPaths>
#include <QVector>

#include <algorithm>

using namespace KUserFeedback::Console;

QVector<Product> SchemaEntryTemplates::availableTemplates()
{
    QVector<Product> templates;

    auto dirs = QStandardPaths::locateAll(QStandardPaths::GenericDataLocation, QStringLiteral("org.kde.user-feedback/schematemplates"), QStandardPaths::LocateDirectory);
    dirs += QStringLiteral(":/org.kde.user-feedback/schematemplates");
    foreach (const auto &dir, dirs) {
        QDirIterator it(dir, {QStringLiteral("*.schema")}, QDir::Files | QDir::Readable);
        while (it.hasNext()) {
            const auto fileName = it.next();
            QFile f(fileName);
            if (!f.open(QFile::ReadOnly))
                continue;
            const auto ps = Product::fromJson(f.readAll());
            if (ps.isEmpty())
                qWarning() << "Failed to read template" << fileName;
            templates += ps;
        }
    }

    std::sort(templates.begin(), templates.end(), [](const auto &lhs, const auto &rhs) {
        return lhs.name() < rhs.name();
    });

    return templates;
}
