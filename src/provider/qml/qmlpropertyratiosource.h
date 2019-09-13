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

#ifndef KUSERFEEDBACK_QMLPROPERTYRATIOSOURCE_H
#define KUSERFEEDBACK_QMLPROPERTYRATIOSOURCE_H

#include "qmlabstractdatasource.h"

namespace KUserFeedback {

class PropertyRatioSource;

class QmlPropertyRatioSource : public QmlAbstractDataSource
{
    Q_OBJECT
    Q_PROPERTY(QString sourceId READ sourceId WRITE setSourceId NOTIFY changed)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY changed)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY changed)
    Q_PROPERTY(QObject* object READ object WRITE setObject NOTIFY changed)
    Q_PROPERTY(QString propertyName READ propertyName WRITE setPropertyName NOTIFY changed)
public:
    explicit QmlPropertyRatioSource(QObject *parent = nullptr);
    ~QmlPropertyRatioSource();

    QString sourceId() const;
    void setSourceId(const QString &id);

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
