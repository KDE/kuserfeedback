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
