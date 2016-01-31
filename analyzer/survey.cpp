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
};

}
}

Survey::Survey() : d(new SurveyData) {}
Survey::Survey(const Survey&) = default;
Survey::~Survey() = default;
Survey& Survey::operator=(const Survey&) = default;

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

QByteArray Survey::toJson() const
{
    QJsonObject obj;
    obj.insert(QStringLiteral("name"), name());
    obj.insert(QStringLiteral("url"), url().toString());
    return QJsonDocument(obj).toJson();
}

QVector<Survey> Survey::fromJson(const QByteArray &data)
{
    QVector<Survey> surveys;
    foreach (const auto &v, QJsonDocument::fromJson(data).array()) {
        const auto obj = v.toObject();
        Survey survey;
        survey.setName(obj.value(QStringLiteral("name")).toString());
        survey.setUrl(QUrl(obj.value(QStringLiteral("url")).toString()));
        surveys.push_back(survey);
    }
    return surveys;
}
