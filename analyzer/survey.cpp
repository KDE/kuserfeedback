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

using namespace UserFeedback::Analyzer;

namespace UserFeedback {
namespace Analyzer {

class SurveyData : public QSharedData
{
public:
    QString name;
    QUrl url;
    int id = -1;
    bool active = false;
};

}
}

Survey::Survey() : d(new SurveyData) {}
Survey::Survey(const Survey&) = default;
Survey::~Survey() = default;
Survey& Survey::operator=(const Survey&) = default;

int Survey::id() const
{
    return d->id;
}

void Survey::setId(int id)
{
    d->id = id;
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

QByteArray Survey::toJson() const
{
    QJsonObject obj;
    obj.insert(QStringLiteral("id"), id());
    obj.insert(QStringLiteral("name"), name());
    obj.insert(QStringLiteral("url"), url().toString());
    obj.insert(QStringLiteral("active"), isActive());
    return QJsonDocument(obj).toJson();
}

QVector<Survey> Survey::fromJson(const QByteArray &data)
{
    QVector<Survey> surveys;
    foreach (const auto &v, QJsonDocument::fromJson(data).array()) {
        const auto obj = v.toObject();
        Survey survey;

        const auto id = obj.value(QStringLiteral("id"));
        // TODO move this to a helper function
        if (id.isDouble())
            survey.setId(id.toInt(-1));
        else if (id.isString())
            survey.setId(id.toString().toInt());
        survey.setName(obj.value(QStringLiteral("name")).toString());
        survey.setUrl(QUrl(obj.value(QStringLiteral("url")).toString()));
        survey.setActive(obj.value(QStringLiteral("active")).toBool());
        surveys.push_back(survey);
    }
    return surveys;
}
