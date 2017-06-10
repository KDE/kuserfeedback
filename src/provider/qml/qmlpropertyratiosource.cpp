/*
    Copyright (C) 2017 Volker Krause <vkrause@kde.org>

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

QString QmlPropertyRatioSource::name() const
{
    return source()->name();
}

void QmlPropertyRatioSource::setName(const QString& name)
{
    if (source()->name() == name)
        return;
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
