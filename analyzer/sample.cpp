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

#include "sample.h"

#include <QDateTime>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSharedData>

using namespace UserFeedback::Analyzer;

class UserFeedback::Analyzer::SampleData : public QSharedData
{
public:
    QDateTime timestamp;
    int startCount;
    int usageTime;
    QString version;
};

Sample::Sample() : d(new SampleData) {}
Sample::Sample(const Sample&) = default;
Sample::~Sample() = default;
Sample& Sample::operator=(const Sample&) = default;

QDateTime Sample::timestamp() const
{
    return d->timestamp;
}

void Sample::setTimestamp(const QDateTime& timestamp)
{
    d->timestamp = timestamp;
}

int Sample::startCount() const
{
    return d->startCount;
}

void Sample::setStartCount(int startCount)
{
    d->startCount = startCount;
}

int Sample::usageTime() const
{
    return d->usageTime;
}

void Sample::setUsageTime(int usageTime)
{
    d->usageTime = usageTime;
}

QString Sample::version() const
{
    return d->version;
}

void Sample::setVersion(const QString& version)
{
    d->version = version;
}

QVector<Sample> Sample::fromJson(const QByteArray &json)
{
    const auto array = QJsonDocument::fromJson(json).array();
    QVector<Sample> samples;
    samples.reserve(array.size());
    for (auto it = array.begin(); it != array.end(); ++it) {
        const auto obj = it->toObject();
        Sample s;
        s.setTimestamp(QDateTime::fromString(obj.value(QStringLiteral("timestamp")).toString(), Qt::ISODate));
        s.setStartCount(obj.value(QStringLiteral("startCount")).toInt());
        s.setUsageTime(obj.value(QStringLiteral("usageTime")).toInt());
        s.setVersion(obj.value(QStringLiteral("version")).toString());
        samples.push_back(s);
    }
    return samples;
}