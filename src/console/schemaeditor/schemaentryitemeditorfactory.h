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

#ifndef KUSERFEEDBACK_CONSOLE_SCHEMAENTRYITEMEDITORFACTORY_H
#define KUSERFEEDBACK_CONSOLE_SCHEMAENTRYITEMEDITORFACTORY_H

#include <QItemEditorFactory>

#include <memory>

namespace KUserFeedback {
namespace Console {

class AggregationElementModel;
class Product;

class SchemaEntryItemEditorFactory : public QItemEditorFactory
{
public:
    SchemaEntryItemEditorFactory();
    ~SchemaEntryItemEditorFactory();

    void setProduct(const Product &product);

private:
    Q_DISABLE_COPY(SchemaEntryItemEditorFactory)
    std::unique_ptr<AggregationElementModel> m_elementModel;
};


}
}

#endif // KUSERFEEDBACK_CONSOLE_SCHEMAENTRYITEMEDITORFACTORY_H
