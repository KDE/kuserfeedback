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

#include "qmlpropertysource.h"
#include <abstractdatasource.h>

using namespace KUserFeedback;

class CustomPropertySource : public AbstractDataSource
{
public:
    CustomPropertySource() : AbstractDataSource({}) {}

    QVariant data() override { return m_data; }
    QString name() const override { return m_name; }
    QString description() const override { return m_description; }

    void setSourceId(const QString &id) { setId(id); }

    QVariant m_data;
    QString m_name;
    QString m_description;
};

QmlPropertySource::QmlPropertySource(QObject* parent)
    : QmlAbstractDataSource(new CustomPropertySource(), parent)
{
}

QVariant QmlPropertySource::data() const
{
    return source()->data();
}

QString QmlPropertySource::name() const
{
    return source()->name();
}

QString QmlPropertySource::description() const
{
    return source()->description();
}

QString QmlPropertySource::sourceId() const
{
    return source()->id();
}

void QmlPropertySource::setData(const QVariant& data)
{
    if (customSource()->m_data != data) {
        customSource()->m_data = data;
        Q_EMIT dataChanged(data);
    }
}

void QmlPropertySource::setName(const QString &name)
{
    if (customSource()->m_name != name) {
        customSource()->m_name = name;
        Q_EMIT nameChanged(name);
    }
}

void QmlPropertySource::setDescription(const QString& description)
{
    if (customSource()->m_description != description) {
        customSource()->m_description = description;
        Q_EMIT descriptionChanged(description);
    }
}

void QmlPropertySource::setSourceId(const QString& id)
{
    if (id != source()->id()) {
        customSource()->setSourceId(id);
        Q_EMIT idChanged(id);
    }
}

CustomPropertySource * KUserFeedback::QmlPropertySource::customSource()
{
    return dynamic_cast<CustomPropertySource*>(source());
}
