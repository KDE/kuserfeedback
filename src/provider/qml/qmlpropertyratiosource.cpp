/*
    Copyright (C) 2017 Volker Krause <vkrause@kde.org>

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

#include "qmlpropertyratiosource.h"

#include <PropertyRatioSource>

using namespace KUserFeedback;

QmlPropertyRatioSource::QmlPropertyRatioSource(QObject* parent)
    : QmlAbstractDataSource(new PropertyRatioSource(nullptr, nullptr, QString()), parent)
{
}

QmlPropertyRatioSource::~QmlPropertyRatioSource()
{
}

PropertyRatioSource* QmlPropertyRatioSource::prSrc() const
{
    return static_cast<PropertyRatioSource*>(source());
}

QString QmlPropertyRatioSource::sourceId() const
{
    return source()->id();
}

void QmlPropertyRatioSource::setSourceId(const QString& id)
{
    if (source()->id() == id)
        return;
    prSrc()->setId(id);
    emit changed();
}

QString QmlPropertyRatioSource::name() const
{
    return prSrc()->name();
}

void QmlPropertyRatioSource::setName(const QString& name)
{
    if (prSrc()->name() == name) {
        return;
    }
    prSrc()->setName(name);
    emit changed();
}

QString QmlPropertyRatioSource::description() const
{
    return prSrc()->description();
}

void QmlPropertyRatioSource::setDescription(const QString& desc)
{
    if (description() == desc)
        return;
    prSrc()->setDescription(desc);
    emit changed();
}

QObject* QmlPropertyRatioSource::object() const
{
    return prSrc()->object();
}

void QmlPropertyRatioSource::setObject(QObject* object)
{
    if (prSrc()->object() == object)
        return;
    prSrc()->setObject(object);
    emit changed();
}

QString QmlPropertyRatioSource::propertyName() const
{
    return prSrc()->propertyName();
}

void QmlPropertyRatioSource::setPropertyName(const QString& name)
{
    if (propertyName() == name)
        return;
    prSrc()->setPropertyName(name);
    emit changed();
}

void QmlPropertyRatioSource::addValueMapping(const QVariant& value, const QString& name)
{
    prSrc()->addValueMapping(value, name);
}
