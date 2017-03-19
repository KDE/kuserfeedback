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

#ifndef USERFEEDBACK_SURVEYINFO_H
#define USERFEEDBACK_SURVEYINFO_H

#include "userfeedbackcore_export.h"

#include <QSharedDataPointer>

class QJsonObject;
class QUrl;

namespace UserFeedback {

class SurveyInfoData;

/*! Information about a survey request.
 *  This class is implicitly shared.
 */
class USERFEEDBACKCORE_EXPORT SurveyInfo
{
public:
    /*! Create an empty, invalid survey request. */
    SurveyInfo();
    /*! Copy constructor. */
    SurveyInfo(const SurveyInfo&);
    ~SurveyInfo();
    /*! Assignment operator. */
    SurveyInfo& operator=(const SurveyInfo&);

    /*! Returns @c true if this survey has all necessary information to actually execute it. */
    bool isValid() const;

    /*! Internal unique id of the survey.
     *  Used to locally check if a user has completed the survey already.
     */
    int id() const;

    /*! The URL to the survey website. */
    QUrl url() const;

    /*! The survey targeting expression. */
    QString target() const;

    ///@cond internal
    void setId(int id);
    void setUrl(const QUrl &url);
    void setTarget(const QString &target);
    static SurveyInfo fromJson(const QJsonObject &obj);
    ///@endcond
private:
    QSharedDataPointer<SurveyInfoData> d;
};

}

#endif // USERFEEDBACK_SURVEYINFO_H
