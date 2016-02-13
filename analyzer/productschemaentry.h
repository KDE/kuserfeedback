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

#ifndef USERFEEDBACK_ANALYZER_PRODUCTSCHEMAENTRY_H
#define USERFEEDBACK_ANALYZER_PRODUCTSCHEMAENTRY_H

#include <QObject>
#include <QSharedDataPointer>
#include <QVector>

class QJsonArray;
class QJsonObject;

namespace UserFeedback {
namespace Analyzer {

class ProductSchemaEntryData;

class ProductSchemaEntry
{
    Q_GADGET
public:
    enum Type {
        InvalidType,
        IntegerType,
        StringType,
        StringListType
    };
    Q_ENUM(Type)

    ProductSchemaEntry();
    ProductSchemaEntry(const ProductSchemaEntry &entry);
    ~ProductSchemaEntry();
    ProductSchemaEntry& operator=(const ProductSchemaEntry &entry);

    int internalId() const;
    void setInternalId(int internalId);

    QString name() const;
    void setName(const QString& name);

    Type type() const;
    void setType(Type type);

    static QString displayString(Type type);

    QJsonObject toJsonObject() const;
    static QVector<ProductSchemaEntry> fromJson(const QJsonArray &array);

private:
    QSharedDataPointer<ProductSchemaEntryData> d;
};

}
}

Q_DECLARE_METATYPE(UserFeedback::Analyzer::ProductSchemaEntry)
Q_DECLARE_METATYPE(UserFeedback::Analyzer::ProductSchemaEntry::Type)
Q_DECLARE_TYPEINFO(UserFeedback::Analyzer::ProductSchemaEntry, Q_MOVABLE_TYPE);

#endif // USERFEEDBACK_ANALYZER_PRODUCTSCHEMAENTRY_H
