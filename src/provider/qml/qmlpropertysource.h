/*
    Copyright (C) 2017 Aleix Pol Gonzalez <aleixpol@kde.org>

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
    Q_PROPERTY(QString sourceId READ sourceId WRITE setSourceId NOTIFY idChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QVariant data READ data WRITE setData NOTIFY dataChanged)
public:
    QmlPropertySource(QObject* parent = nullptr);

    QString sourceId() const;
    QString name() const;
    QString description() const;
    QVariant data() const;

    void setSourceId(const QString &id);
    void setName(const QString &name);
    void setDescription(const QString &description);
    void setData(const QVariant &data);

Q_SIGNALS:
    void idChanged(const QString &id);
    void nameChanged(const QString &name);
    void descriptionChanged(const QString &description);
    void dataChanged(const QVariant &data);

private:
    CustomPropertySource* customSource();
};
}

#endif // KUSERFEEDBACK_QTVERSIONSOURCE_H
