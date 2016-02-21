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

#include "ratioset.h"

#include <QDebug>
#include <QHash>
#include <QJsonObject>
#include <QSharedData>
#include <QString>
#include <QVector>

#include <algorithm>

using namespace UserFeedback::Analyzer;

namespace UserFeedback {
namespace Analyzer {
class RatioSetData : public QSharedData
{
public:
    QHash<QString, double> ratioSet;
};

}
}

RatioSet::RatioSet() : d(new RatioSetData)
{
}

RatioSet::RatioSet(const Analyzer::RatioSet&) = default;
RatioSet::~RatioSet() = default;
RatioSet& RatioSet::operator=(const RatioSet&) = default;

QString RatioSet::displayString() const
{
    QVector<QPair<QString, double>> data;
    data.reserve(d->ratioSet.size());
    for (auto it = d->ratioSet.constBegin(); it != d->ratioSet.constEnd(); ++it)
        data.push_back(qMakePair(it.key(), it.value()));

    std::sort(data.begin(), data.end(), [](const QPair<QString, double> &lhs, const QPair<QString, double> &rhs) {
        return rhs.second < lhs.second;
    });

    QStringList l;
    l.reserve(data.size());
    for (auto it = data.constBegin(); it != data.constEnd(); ++it)
        l.push_back((*it).first + QStringLiteral(": ") + QString::number((*it).second * 100.0, 'f', 1) + QLatin1Char('%'));

    return l.join(QStringLiteral(", "));
}

QHash<QString, double> RatioSet::data() const
{
    return d->ratioSet;
}

RatioSet RatioSet::fromJson(const QJsonObject& obj)
{
    RatioSet set;
    for (auto it = obj.begin(); it != obj.end(); ++it) {
        set.d->ratioSet.insert(it.key(), it.value().toDouble());
    }
    return set;
}
