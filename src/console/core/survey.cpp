/*
    Copyright (C) 2016 Volker Krause <vkrause@kde.org>

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

#include "survey.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSharedData>
#include <QString>
#include <QUrl>
#include <QUuid>

using namespace KUserFeedback::Console;

namespace KUserFeedback {
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
