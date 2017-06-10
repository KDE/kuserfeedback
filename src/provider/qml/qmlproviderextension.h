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

#ifndef KUSERFEEDBACK_QMLPROVIDEREXTENSION_H
#define KUSERFEEDBACK_QMLPROVIDEREXTENSION_H

#include "qmlabstractdatasource.h"

#include <QObject>
#include <QQmlListProperty>
#include <QVector>

namespace KUserFeedback {

class QmlProviderExtension : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<KUserFeedback::QmlAbstractDataSource> sources READ sources)
    Q_CLASSINFO("DefaultProperty", "sources")

public:
    explicit QmlProviderExtension(QObject *parent = nullptr);
    ~QmlProviderExtension();

    QQmlListProperty<QmlAbstractDataSource> sources();

private:
    static void sourceAppend(QQmlListProperty<QmlAbstractDataSource> *prop, QmlAbstractDataSource *value);
    static int sourceCount(QQmlListProperty<QmlAbstractDataSource> *prop);
    static QmlAbstractDataSource* sourceAt(QQmlListProperty<QmlAbstractDataSource> *prop, int index);
    static void sourceClear(QQmlListProperty<QmlAbstractDataSource> *prop);

    QVector<QmlAbstractDataSource*> m_sourceWrappers;
};

}

#endif // KUSERFEEDBACK_QMLPROVIDEREXTENSION_H
