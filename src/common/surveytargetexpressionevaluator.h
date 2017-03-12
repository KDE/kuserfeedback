/*
    Copyright (C) 2017 Volker Krause <vkrause@kde.org>

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

#ifndef USERFEEDBACK_SURVEYTARGETEXPRESSIONEVALUATOR_H
#define USERFEEDBACK_SURVEYTARGETEXPRESSIONEVALUATOR_H

#include <QHash>

class QString;
class QVariant;

namespace UserFeedback {

class SurveyTargetExpression;

class SurveyTargetExpressionDataProvider
{
public:
    virtual QVariant sourceData(const QString &sourceName) const = 0;
};

class SurveyTargetExpressionEvaluator
{
public:
    SurveyTargetExpressionEvaluator();
    ~SurveyTargetExpressionEvaluator();

    void setDataProvider(SurveyTargetExpressionDataProvider *provider);

    bool evaluate(SurveyTargetExpression *expression);

private:
    QVariant value(SurveyTargetExpression *expression);
    QVariant value(const QString &source);

    SurveyTargetExpressionDataProvider *m_provider;
    QHash<QString, QVariant> m_dataCache;
};

}

#endif // USERFEEDBACK_SURVEYTARGETEXPRESSIONEVALUATOR_H
