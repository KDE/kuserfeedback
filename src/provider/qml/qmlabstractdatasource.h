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

#ifndef KUSERFEEDBACK_QMLABSTRACTDATASOURCE_H
#define KUSERFEEDBACK_QMLABSTRACTDATASOURCE_H

#include <provider.h>

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
