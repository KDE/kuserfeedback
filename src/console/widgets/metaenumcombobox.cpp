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

#include "metaenumcombobox.h"

#include <QDebug>
#include <QMetaEnum>
#include <QMetaType>

using namespace KUserFeedback::Console;

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
