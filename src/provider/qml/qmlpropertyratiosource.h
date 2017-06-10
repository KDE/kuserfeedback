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

#ifndef KUSERFEEDBACK_QMLPROPERTYRATIOSOURCE_H
#define KUSERFEEDBACK_QMLPROPERTYRATIOSOURCE_H

#include "qmlabstractdatasource.h"

namespace KUserFeedback {

class PropertyRatioSource;

class QmlPropertyRatioSource : public QmlAbstractDataSource
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY changed)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY changed)
    Q_PROPERTY(QObject* object READ object WRITE setObject NOTIFY changed)
    Q_PROPERTY(QString propertyName READ propertyName WRITE setPropertyName NOTIFY changed)
public:
    explicit QmlPropertyRatioSource(QObject *parent = nullptr);
    ~QmlPropertyRatioSource();

    QString name() const;
    void setName(const QString &name);

    QString description() const;
    void setDescription(const QString &desc);

    QObject* object() const;
    void setObject(QObject *object);

    QString propertyName() const;
    void setPropertyName(const QString &name);

    Q_INVOKABLE void addValueMapping(const QVariant &value, const QString &name);

Q_SIGNALS:
    void changed();

private:
    PropertyRatioSource* prSrc() const;
};

}

#endif // KUSERFEEDBACK_QMLPROPERTYRATIOSOURCE_H
