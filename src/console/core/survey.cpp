/*
    Copyright (C) 2016 Volker Krause <vkrause@kde.org>

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

#include "survey.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSharedData>
#include <QString>
#include <QUrl>
#include <QUuid>

using namespace UserFeedback::Console;

namespace UserFeedback {
namespace Console {

class SurveyData : public QSharedData
{
public:
    QUuid uuid;
    QString name;
    QUrl url;
    QString target;
    bool active = false;
};

}
}

Survey::Survey() : d(new SurveyData) {}
Survey::Survey(const Survey&) = default;
Survey::~Survey() = default;
Survey& Survey::operator=(const Survey&) = default;

bool Survey::operator==(const Survey& other) const
{
    return d->name == other.d->name
        && d->url == other.d->url
        && d->target == other.d->target
        && d->uuid == other.d->uuid
        && d->active == other.d->active;
}

bool Survey::operator!=(const Survey& other) const
{
    return !(*this == other);
}

QUuid Survey::uuid() const
{
    return d->uuid;
}

void Survey::setUuid(const QUuid &id)
{
    d->uuid = id;
}

QString Survey::name() const
{
    return d->name;
}

void Survey::setName(const QString& name)
{
    d->name = name;
}

QUrl Survey::url() const
{
    return d->url;
}

void Survey::setUrl(const QUrl& url)
{
    d->url = url;
}

bool Survey::isActive() const
{
    return d->active;
}

void Survey::setActive(bool active)
{
    d->active = active;
}

QString Survey::target() const
{
    return d->target;
}

void Survey::setTarget(const QString& target)
{
    d->target = target;
}

QByteArray Survey::toJson() const
{
    QJsonObject obj;
    obj.insert(QStringLiteral("uuid"), uuid().toString());
    obj.insert(QStringLiteral("name"), name());
    obj.insert(QStringLiteral("url"), url().toString());
    obj.insert(QStringLiteral("active"), isActive());
    obj.insert(QStringLiteral("target"), target());
    return QJsonDocument(obj).toJson();
}

QVector<Survey> Survey::fromJson(const QByteArray &data)
{
    QVector<Survey> surveys;
    foreach (const auto &v, QJsonDocument::fromJson(data).array()) {
        const auto obj = v.toObject();
        Survey survey;
        survey.setUuid(QUuid(obj.value(QLatin1String("uuid")).toString()));
        survey.setName(obj.value(QLatin1String("name")).toString());
        survey.setUrl(QUrl(obj.value(QLatin1String("url")).toString()));
        survey.setActive(obj.value(QLatin1String("active")).toBool());
        survey.setTarget(obj.value(QLatin1String("target")).toString());
        surveys.push_back(survey);
    }
    return surveys;
}
