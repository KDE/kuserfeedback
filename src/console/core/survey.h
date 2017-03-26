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

#ifndef USERFEEDBACK_CONSOLE_SURVEY_H
#define USERFEEDBACK_CONSOLE_SURVEY_H

#include <QMetaType>
#include <QSharedDataPointer>
#include <QVector>

class QString;
class QUrl;
class QUuid;

namespace UserFeedback {
namespace Console {

class SurveyData;

/** Data for one survey. */
class Survey
{
public:
    Survey();
    Survey(const Survey&);
    ~Survey();
    Survey& operator=(const Survey&);

    bool operator==(const Survey &other) const;
    bool operator!=(const Survey &other) const;

    QUuid uuid() const;
    void setUuid(const QUuid &id);

    QString name() const;
    void setName(const QString& name);

    QUrl url() const;
    void setUrl(const QUrl& url);

    bool isActive() const;
    void setActive(bool enabled);

    QString target() const;
    void setTarget(const QString &target);

    QByteArray toJson() const;
    static QVector<Survey> fromJson(const QByteArray &data);
private:
    QSharedDataPointer<SurveyData> d;
};

}
}

Q_DECLARE_TYPEINFO(UserFeedback::Console::Survey, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(UserFeedback::Console::Survey)

#endif // USERFEEDBACK_CONSOLE_SURVEY_H
