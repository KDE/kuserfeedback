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

#include "metaenumcombobox.h"

#include <QDebug>
#include <QMetaEnum>
#include <QMetaType>

using namespace UserFeedback::Analyzer;

MetaEnumComboBox::MetaEnumComboBox(QWidget* parent) :
    QComboBox(parent)
{
}

MetaEnumComboBox::~MetaEnumComboBox()
{
}

QVariant MetaEnumComboBox::value() const
{
    auto value = m_value;
    *(static_cast<int*>(value.data())) = currentData().toInt();
    return value;
}

void MetaEnumComboBox::setValue(const QVariant& value)
{
    clear();
    m_value = value;

    const auto mo = QMetaType::metaObjectForType(value.userType());
    if (!mo)
        return;

    const QByteArray typeName = value.typeName();
    const auto idx = typeName.lastIndexOf("::");
    if (idx <= 0)
        return;

    const auto enumName = typeName.mid(idx + 2);
    const auto enumIdx = mo->indexOfEnumerator(enumName);
    if (enumIdx < 0)
        return;

    const auto me = mo->enumerator(enumIdx);
    for (int i = 0; i < me.keyCount(); ++i) {
        addItem(QLatin1String(me.key(i)), me.value(i));
        if (me.value(i) == value.toInt())
            setCurrentIndex(i);
    }
}
