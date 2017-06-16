/*
    Copyright (C) 2017 Aleix Pol Gonzalez <aleixpol@kde.org>

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

#ifndef KUSERFEEDBACK_QMLPROPERTYSOURCE_H
#define KUSERFEEDBACK_QMLPROPERTYSOURCE_H

#include "qmlabstractdatasource.h"
#include <QVariant>

class CustomPropertySource;

namespace KUserFeedback {

/*! Data source reporting information provided by a QML application
 */
class QmlPropertySource : public QmlAbstractDataSource
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QVariant data READ data WRITE setData NOTIFY dataChanged)
public:
    QmlPropertySource(QObject* parent = nullptr);

    QString name() const;
    QString description() const;
    QVariant data() const;

    void setName(const QString &name);
    void setDescription(const QString &description);
    void setData(const QVariant &data);

Q_SIGNALS:
    void nameChanged(const QString &name);
    void descriptionChanged(const QString &description);
    void dataChanged(const QVariant &data);

private:
    CustomPropertySource* customSource();
};
}

#endif // KUSERFEEDBACK_QTVERSIONSOURCE_H
