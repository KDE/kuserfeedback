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

#include "qmlproviderextension.h"

using namespace KUserFeedback;

QmlProviderExtension::QmlProviderExtension(QObject* parent) :
    QObject(parent)
{
}

QmlProviderExtension::~QmlProviderExtension()
{
}

void QmlProviderExtension::sourceAppend(QQmlListProperty<QmlAbstractDataSource> *prop, QmlAbstractDataSource *value)
{
    static_cast<QmlProviderExtension*>(prop->object)->m_sourceWrappers.push_back(value);
    static_cast<Provider*>(prop->object->parent())->addDataSource(value->source());
}

int QmlProviderExtension::sourceCount(QQmlListProperty<QmlAbstractDataSource> *prop)
{
    return static_cast<QmlProviderExtension*>(prop->object)->m_sourceWrappers.size();
}

QmlAbstractDataSource* QmlProviderExtension::sourceAt(QQmlListProperty<QmlAbstractDataSource> *prop, int index)
{
    return static_cast<QmlProviderExtension*>(prop->object)->m_sourceWrappers.at(index);
}

void QmlProviderExtension::sourceClear(QQmlListProperty<QmlAbstractDataSource> *prop)
{
    static_cast<QmlProviderExtension*>(prop->object)->m_sourceWrappers.clear();
}

QQmlListProperty<QmlAbstractDataSource> QmlProviderExtension::sources()
{
    return QQmlListProperty<QmlAbstractDataSource>(this, nullptr,
        QmlProviderExtension::sourceAppend, QmlProviderExtension::sourceCount,
        QmlProviderExtension::sourceAt, QmlProviderExtension::sourceClear);
}
