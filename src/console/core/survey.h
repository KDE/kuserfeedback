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

#ifndef KUSERFEEDBACK_CONSOLE_SURVEY_H
#define KUSERFEEDBACK_CONSOLE_SURVEY_H

#include <QMetaType>
#include <QSharedDataPointer>
#include <QVector>

class QString;
class QUrl;
class QUuid;

namespace KUserFeedback {
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

Q_DECLARE_TYPEINFO(KUserFeedback::Console::Survey, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(KUserFeedback::Console::Survey)

#endif // KUSERFEEDBACK_CONSOLE_SURVEY_H
