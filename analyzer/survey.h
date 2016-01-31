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

#ifndef USERFEEDBACK_ANALYZER_SURVEY_H
#define USERFEEDBACK_ANALYZER_SURVEY_H

#include <QSharedDataPointer>
#include <QVector>

class QString;
class QUrl;

namespace UserFeedback {
namespace Analyzer {

class SurveyData;

/** Data for one survey. */
class Survey
{
public:
    Survey();
    Survey(const Survey&);
    ~Survey();
    Survey& operator=(const Survey&);

    QString name() const;
    void setName(const QString& name);

    QUrl url() const;
    void setUrl(const QUrl& url);

    QByteArray toJson() const;
    static QVector<Survey> fromJson(const QByteArray &data);
private:
    QSharedDataPointer<SurveyData> d;
};

}
}

Q_DECLARE_TYPEINFO(UserFeedback::Analyzer::Survey, Q_MOVABLE_TYPE);

#endif // USERFEEDBACK_ANALYZER_SURVEY_H
