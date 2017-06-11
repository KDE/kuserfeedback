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

#ifndef KUSERFEEDBACK_QMLABSTRACTDATASOURCE_H
#define KUSERFEEDBACK_QMLABSTRACTDATASOURCE_H

#include <Provider>

namespace KUserFeedback {

class AbstractDataSource;

class QmlAbstractDataSource : public QObject
{
    Q_OBJECT
    Q_PROPERTY(KUserFeedback::Provider::TelemetryMode mode READ telemetryMode
               WRITE setTelemetryMode NOTIFY telemetryModeChanged)
public:
    explicit QmlAbstractDataSource(AbstractDataSource *source, QObject *parent);
    ~QmlAbstractDataSource();

    Provider::TelemetryMode telemetryMode() const;
    void setTelemetryMode(Provider::TelemetryMode mode);

    AbstractDataSource* source() const;

Q_SIGNALS:
    void telemetryModeChanged();

private:
    AbstractDataSource *m_source;
};

}

#endif // KUSERFEEDBACK_QMLABSTRACTDATASOURCE_H
