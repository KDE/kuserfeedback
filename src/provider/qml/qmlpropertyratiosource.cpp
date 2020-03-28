/*
    SPDX-FileCopyrightText: 2017 Volker Krause <vkrause@kde.org>

    SPDX-License-Identifier: MIT
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
