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

#include "surveyinfo.h"

#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QJsonObject>
#endif
#include <QSharedData>
#include <QUrl>

using namespace UserFeedback;

class UserFeedback::SurveyInfoData : public QSharedData
{
public:
    SurveyInfoData();

    int id;
    QUrl url;
};

SurveyInfoData::SurveyInfoData()
    : id(-1)
{
}


SurveyInfo::SurveyInfo() : d (new SurveyInfoData)
{
}

SurveyInfo::SurveyInfo(const SurveyInfo&) = default;

SurveyInfo::~SurveyInfo() = default;

SurveyInfo & SurveyInfo::operator=(const SurveyInfo&) = default;

bool SurveyInfo::isValid() const
{
    return d->id >= 0 && d->url.isValid();
}

int SurveyInfo::id() const
{
    return d->id;
}

void SurveyInfo::setId(int id)
{
    d->id = id;
}

QUrl SurveyInfo::url() const
{
    return d->url;
}

void SurveyInfo::setUrl(const QUrl& url)
{
    d->url = url;
}

SurveyInfo SurveyInfo::fromJson(const QJsonObject& obj)
{
    SurveyInfo s;
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    s.setId(obj.value(QStringLiteral("id")).toInt());
    s.setUrl(QUrl(obj.value(QStringLiteral("url")).toString()));
#endif
    return s;
}
