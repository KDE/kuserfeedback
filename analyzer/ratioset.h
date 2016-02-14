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

#ifndef USERFEEDBACK_ANALYZER_RATIOSET_H
#define USERFEEDBACK_ANALYZER_RATIOSET_H

#include <QSharedDataPointer>
#include <QMetaType>

class QJsonObject;

namespace UserFeedback {
namespace Analyzer {

class RatioSetData;

class RatioSet
{
public:
    RatioSet();
    RatioSet(const RatioSet&);
    ~RatioSet();
    RatioSet& operator=(const RatioSet&);

    QString displayString() const;

    static RatioSet fromJson(const QJsonObject &obj);

private:
    QSharedDataPointer<RatioSetData> d;
};

}
}

Q_DECLARE_METATYPE(UserFeedback::Analyzer::RatioSet)
Q_DECLARE_TYPEINFO(UserFeedback::Analyzer::RatioSet, Q_MOVABLE_TYPE);

#endif // USERFEEDBACK_ANALYZER_RATIOSET_H
