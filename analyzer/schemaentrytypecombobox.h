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

#ifndef USERFEEDBACK_ANALYZER_SCHEMAENTRYTYPECOMBOBOX_H
#define USERFEEDBACK_ANALYZER_SCHEMAENTRYTYPECOMBOBOX_H

#include "productschemaentry.h"

#include <QComboBox>

namespace UserFeedback {
namespace Analyzer {

class SchemaEntryTypeComboBox : public QComboBox
{
    Q_OBJECT
    Q_PROPERTY(UserFeedback::Analyzer::ProductSchemaEntry::Type type READ type WRITE setType USER true)
public:
    explicit SchemaEntryTypeComboBox(QWidget *parent = nullptr);
    ~SchemaEntryTypeComboBox();

    ProductSchemaEntry::Type type() const;
    void setType(ProductSchemaEntry::Type type);

};

}
}

#endif // USERFEEDBACK_ANALYZER_SCHEMAENTRYTYPECOMBOBOX_H
