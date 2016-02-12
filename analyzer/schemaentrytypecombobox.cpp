/*
    Copyright (C) 2016 Volker Krause <volker.krause@kdab.com>

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

#include "schemaentrytypecombobox.h"

#include <QDebug>

using namespace UserFeedback::Analyzer;

SchemaEntryTypeComboBox::SchemaEntryTypeComboBox(QWidget* parent) :
    QComboBox(parent)
{
    addItem(tr("Integer"), QVariant::fromValue(ProductSchemaEntry::IntegerType));
    addItem(tr("String"), QVariant::fromValue(ProductSchemaEntry::StringType));
}

SchemaEntryTypeComboBox::~SchemaEntryTypeComboBox() = default;

void SchemaEntryTypeComboBox::setType(ProductSchemaEntry::Type type)
{
    qDebug() << type;
    setCurrentIndex(findData(QVariant::fromValue(type)));
}

ProductSchemaEntry::Type SchemaEntryTypeComboBox::type() const
{
    qDebug() << currentData();
    return currentData().value<ProductSchemaEntry::Type>();
}
