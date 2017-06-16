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

#include "qmlpropertysource.h"
#include <abstractdatasource.h>

using namespace KUserFeedback;

class CustomPropertySource : public AbstractDataSource
{
public:
    CustomPropertySource() : AbstractDataSource({}) {}

    QVariant data() override { return m_data; }
    QString description() const override { return m_description; }

    void setSourceName(const QString &name) { setName(name); }

    QVariant m_data;
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

QString QmlPropertySource::description() const
{
    return source()->description();
}

QString QmlPropertySource::name() const
{
    return source()->name();
}

void KUserFeedback::QmlPropertySource::setData(const QVariant& data)
{
    if (customSource()->m_data != data) {
        customSource()->m_data = data;
        Q_EMIT dataChanged(data);
    }
}

void KUserFeedback::QmlPropertySource::setDescription(const QString& description)
{
    if (customSource()->m_description != description) {
        customSource()->m_description = description;
        Q_EMIT descriptionChanged(description);
    }
}

void KUserFeedback::QmlPropertySource::setName(const QString& name)
{
    if (name != source()->name()) {
        customSource()->setSourceName(name);
        Q_EMIT nameChanged(name);
    }
}

CustomPropertySource * KUserFeedback::QmlPropertySource::customSource()
{
    return dynamic_cast<CustomPropertySource*>(source());
}
