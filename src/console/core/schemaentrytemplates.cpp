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
