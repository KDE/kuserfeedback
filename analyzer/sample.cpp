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