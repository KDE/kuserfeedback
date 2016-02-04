#ifndef USERFEEDBACK_ANALYZER_SAMPLE_H
#define USERFEEDBACK_ANALYZER_SAMPLE_H

#include <QMetaType>
#include <QSharedDataPointer>
#include <QVector>

class QDateTime;
class QString;

namespace UserFeedback {
namespace Analyzer {

class SampleData;

/** One data sample reported by a client. */
class Sample
{
public:
    Sample();
    Sample(const Sample&);
    ~Sample();
    Sample& operator=(const Sample&);

    QDateTime timestamp() const;
    void setTimestamp(const QDateTime& timestamp);

    int startCount() const;
    void setStartCount(int startCount);

    int usageTime() const;
    void setUsageTime(int usageTime);

    QString version() const;
    void setVersion(const QString& version);

    static QVector<Sample> fromJson(const QByteArray &json);

private:
    QSharedDataPointer<SampleData> d;
};

}
}

Q_DECLARE_TYPEINFO(UserFeedback::Analyzer::Sample, Q_MOVABLE_TYPE);
Q_DECLARE_METATYPE(UserFeedback::Analyzer::Sample)

#endif // USERFEEDBACK_ANALYZER_SAMPLE_H
