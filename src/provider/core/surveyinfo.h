/*
    SPDX-FileCopyrightText: 2016 Volker Krause <vkrause@kde.org>

    SPDX-License-Identifier: MIT
*/

#ifndef KUSERFEEDBACK_SURVEYINFO_H
#define KUSERFEEDBACK_SURVEYINFO_H

#include "kuserfeedbackcore_export.h"

#include <QMetaType>
#include <QSharedDataPointer>
#include <qobjectdefs.h>
#include <QUrl>
#include <QUuid>


QT_BEGIN_NAMESPACE
class QJsonObject;
QT_END_NAMESPACE

namespace KUserFeedback {

class SurveyInfoData;

/*!
 * \class KUserFeedback::SurveyInfo
 * \inmodule KUserFeedbackCore
 * \inheaderfile KUserFeedback/SurveyInfo
 *
 * \brief Information about a survey request.
 *
 * This class is implicitly shared.
 */
class KUSERFEEDBACKCORE_EXPORT SurveyInfo
{
    Q_GADGET

    /*!
     * \property KUserFeedback::SurveyInfo::isValid
     */
    Q_PROPERTY(bool isValid READ isValid)

    /*!
     * \property KUserFeedback::SurveyInfo::url
     */
    Q_PROPERTY(QUrl url READ url)

    /*!
     * \property KUserFeedback::SurveyInfo::target
     */
    Q_PROPERTY(QString target READ target)

    /*!
     * \property KUserFeedback::SurveyInfo::uuid
     */
    Q_PROPERTY(QUuid uuid READ uuid)
public:
    /*! Create an empty, invalid survey request. */
    SurveyInfo();
    SurveyInfo(const SurveyInfo&);
    ~SurveyInfo();
    SurveyInfo& operator=(const SurveyInfo&);

    /*! Returns \c true if this survey has all necessary information to actually execute it. */
    bool isValid() const;

    /*!
     * Internal global unique id of the survey.
     *
     * Used to locally check if a user has completed the survey already.
     */
    QUuid uuid() const;

    /*! The URL to the survey website. */
    QUrl url() const;

    /*! The survey targeting expression. */
    QString target() const;

    void setUuid(const QUuid &id);
    void setUrl(const QUrl &url);
    void setTarget(const QString &target);
    static SurveyInfo fromJson(const QJsonObject &obj);
private:
    QSharedDataPointer<SurveyInfoData> d;
};

}

Q_DECLARE_METATYPE(KUserFeedback::SurveyInfo)

#endif // KUSERFEEDBACK_SURVEYINFO_H
