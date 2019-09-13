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

#include "propertyratiosource.h"
#include "abstractdatasource_p.h"
#include "logging_p.h"

#include <QDebug>
#include <QHash>
#include <QMap>
#include <QMetaProperty>
#include <QObject>
#include <QPointer>
#include <QSettings>
#include <QStringList>
#include <QTime>

using namespace KUserFeedback;

namespace KUserFeedback {
class PropertyRatioSourcePrivate : public AbstractDataSourcePrivate
{
public:
    PropertyRatioSourcePrivate();
    ~PropertyRatioSourcePrivate();

    void propertyChanged();
    QString valueToString(const QVariant &value) const;
    void trySetup();

    QString name;
    QString description;
    QPointer<QObject> obj;
    QByteArray propertyName;
    QObject *signalMonitor;
    QMetaProperty property;
    QString previousValue;
    QTime lastChangeTime;
    QHash<QString, int> ratioSet; // data we are currently tracking
    QHash<QString, int> baseRatioSet; // data loaded from storage
    QMap<QVariant, QString> valueMap;
};

// inefficient workaround for not being able to connect QMetaMethod to a function directly
class SignalMonitor : public QObject
{
    Q_OBJECT
public:
    explicit SignalMonitor(PropertyRatioSourcePrivate *r) : m_receiver(r) {}
public Q_SLOTS:
    void propertyChanged()
    {
        m_receiver->propertyChanged();
    }
private:
    PropertyRatioSourcePrivate *m_receiver;
};

}

PropertyRatioSourcePrivate::PropertyRatioSourcePrivate()
    : obj(nullptr)
    , signalMonitor(nullptr)
{
}

PropertyRatioSourcePrivate::~PropertyRatioSourcePrivate()
{
    delete signalMonitor;
}

void PropertyRatioSourcePrivate::propertyChanged()
{
    if (!previousValue.isEmpty() && lastChangeTime.elapsed() > 1000) {
        ratioSet[previousValue] += lastChangeTime.elapsed() / 1000;
    }

    lastChangeTime.start();
    previousValue = valueToString(property.read(obj));
}

QString PropertyRatioSourcePrivate::valueToString(const QVariant &value) const
{
    const auto it = valueMap.constFind(value);
    if (it != valueMap.constEnd() && it.key() == value) {
        return it.value();
    }
    return value.toString();
}

void PropertyRatioSourcePrivate::trySetup()
{
    if (!obj || propertyName.isEmpty())
        return;

    auto idx = obj->metaObject()->indexOfProperty(propertyName.constData());
    Q_ASSERT(idx >= 0);
    if (idx < 0) {
        qCWarning(Log) << "Property" << propertyName << "not found in" << obj << "!";
        return;
    }

    property = obj->metaObject()->property(idx);
    if (!property.hasNotifySignal()) {
        qCWarning(Log) << "Property" << propertyName << "has no notification signal!";
        return;
    }

    idx = signalMonitor->metaObject()->indexOfMethod("propertyChanged()");
    Q_ASSERT(idx >= 0);
    const auto propertyChangedMethod = signalMonitor->metaObject()->method(idx);
    QObject::connect(obj, property.notifySignal(), signalMonitor, propertyChangedMethod);

    lastChangeTime.start();
    propertyChangedMethod.invoke(signalMonitor, Qt::QueuedConnection);
}

PropertyRatioSource::PropertyRatioSource(QObject *obj, const char *propertyName, const QString &sampleName) :
    AbstractDataSource(sampleName, Provider::DetailedUsageStatistics, new PropertyRatioSourcePrivate)
{
    Q_D(PropertyRatioSource);

    d->obj = obj;
    d->propertyName = propertyName;
    d->signalMonitor = new SignalMonitor(d);
    d->trySetup();
}

QObject* PropertyRatioSource::object() const
{
    Q_D(const PropertyRatioSource);
    return d->obj;
}

void PropertyRatioSource::setObject(QObject* object)
{
    Q_D(PropertyRatioSource);
    if (d->obj == object)
        return;
    d->obj = object;
    d->trySetup();
}

QString PropertyRatioSource::propertyName() const
{
    Q_D(const PropertyRatioSource);
    return QString::fromUtf8(d->propertyName.constData());
}

void PropertyRatioSource::setPropertyName(const QString& name)
{
    Q_D(PropertyRatioSource);
    const auto n = name.toUtf8();
    if (d->propertyName == n)
        return;
    d->propertyName = n;
    d->trySetup();
}

void PropertyRatioSource::addValueMapping(const QVariant &value, const QString &str)
{
    Q_D(PropertyRatioSource);
    d->valueMap.insert(value, str);
}

QString PropertyRatioSource::name() const
{
    Q_D(const PropertyRatioSource);
    return d->name;
}

void PropertyRatioSource::setName(const QString &name)
{
    Q_D(PropertyRatioSource);
    d->name = name;
}

QString PropertyRatioSource::description() const
{
    Q_D(const PropertyRatioSource);
    return d->description;
}

void PropertyRatioSource::setDescription(const QString& desc)
{
    Q_D(PropertyRatioSource);
    d->description = desc;
}

QVariant PropertyRatioSource::data()
{
    Q_D(PropertyRatioSource);
    d->propertyChanged();

    QVariantMap m;
    int total = 0;
    for (auto it = d->ratioSet.constBegin(); it != d->ratioSet.constEnd(); ++it)
        total += it.value() + d->baseRatioSet.value(it.key());
    if (total <= 0)
        return m;

    for (auto it = d->ratioSet.constBegin(); it != d->ratioSet.constEnd(); ++it) {
        double currentValue = it.value() + d->baseRatioSet.value(it.key());
        QVariantMap v;
        v.insert(QStringLiteral("property"), currentValue / (double)(total));
        m.insert(it.key(), v);
    }

    return m;
}

void PropertyRatioSource::loadImpl(QSettings *settings)
{
    Q_D(PropertyRatioSource);
    foreach (const auto &value, settings->childKeys()) {
        const auto amount = std::max(settings->value(value, 0).toInt(), 0);
        d->baseRatioSet.insert(value, amount);
        if (!d->ratioSet.contains(value))
            d->ratioSet.insert(value, 0);
    }
}

void PropertyRatioSource::storeImpl(QSettings *settings)
{
    Q_D(PropertyRatioSource);
    d->propertyChanged();

    // note that a second process can have updated the data meanwhile!
    for (auto it = d->ratioSet.begin(); it != d->ratioSet.end(); ++it) {
        if (it.value() == 0)
            continue;
        const auto oldValue = std::max(settings->value(it.key(), 0).toInt(), 0);
        const auto newValue = oldValue + it.value();
        settings->setValue(it.key(), newValue);
        *it = 0;
        d->baseRatioSet.insert(it.key(), newValue);
    }
}

void PropertyRatioSource::resetImpl(QSettings* settings)
{
    Q_D(PropertyRatioSource);
    d->baseRatioSet.clear();
    d->ratioSet.clear();
    settings->remove(QString());
}

#include "propertyratiosource.moc"
