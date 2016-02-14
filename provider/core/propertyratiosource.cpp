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

#include "propertyratiosource.h"
#include "abstractdatasource_p.h"

#include <QDebug>
#include <QJsonObject>
#include <QMap>
#include <QMetaProperty>
#include <QObject>
#include <QSettings>
#include <QTime>

using namespace UserFeedback;

namespace UserFeedback {
class PropertyRatioSourcePrivate : public AbstractDataSourcePrivate
{
public:
    ~PropertyRatioSourcePrivate();

    void propertyChanged();
    QString valueToString(const QVariant &value) const;

    QObject *obj = nullptr; // TODO make this a QPointer?
    QObject *signalMonitor = nullptr;
    QMetaProperty property;
    QVariant previousValue;
    QTime lastChangeTime;
    QMap<QVariant, int> ratioSet;
    QMap<QVariant, QString> valueMap;
    QString sampleName;
};

// inefficient workaround for not being able to connect QMetaMethod to a function directly
class SignalMonitor : public QObject
{
    Q_OBJECT
public:
    explicit SignalMonitor(PropertyRatioSourcePrivate *r) : m_receiver(r) {}
public slots:
    void propertyChanged()
    {
        m_receiver->propertyChanged();
    }
private:
    PropertyRatioSourcePrivate *m_receiver;
};

}

PropertyRatioSourcePrivate::~PropertyRatioSourcePrivate()
{
    delete signalMonitor;
}

void PropertyRatioSourcePrivate::propertyChanged()
{
    if (previousValue.isValid() && lastChangeTime.elapsed() > 1000) {
        ratioSet[previousValue] += lastChangeTime.elapsed() / 1000;
    }

    lastChangeTime.start();
    previousValue = property.read(obj);
}

QString PropertyRatioSourcePrivate::valueToString(const QVariant &value) const
{
    const auto it = valueMap.constFind(value);
    if (it != valueMap.constEnd())
        return it.value();
    return value.toString();
}

PropertyRatioSource::PropertyRatioSource(QObject *obj, const char *propertyName, const QString &sampleName) :
    AbstractDataSource(new PropertyRatioSourcePrivate)
{
    Q_D(PropertyRatioSource);

    d->obj = obj;
    Q_ASSERT(obj);
    d->sampleName = sampleName;
    Q_ASSERT(!sampleName.isEmpty());

    auto idx = obj->metaObject()->indexOfProperty(propertyName);
    Q_ASSERT(idx >= 0);
    if (idx < 0) {
        qWarning() << "Property" << propertyName << "not found in" << obj << "!";
        return;
    }

    d->property = obj->metaObject()->property(idx);
    if (!d->property.hasNotifySignal()) {
        qWarning() << "Property" << propertyName << "has no notification signal!";
        return;
    }

    d->signalMonitor = new SignalMonitor(d);
    idx = d->signalMonitor->metaObject()->indexOfMethod("propertyChanged()");
    Q_ASSERT(idx >= 0);
    QObject::connect(obj, d->property.notifySignal(), d->signalMonitor, d->signalMonitor->metaObject()->method(idx));

    d->lastChangeTime.start();
    d->propertyChanged();
}

void PropertyRatioSource::addValueMapping(const QVariant &value, const QString &str)
{
    Q_D(PropertyRatioSource);
    d->valueMap.insert(value, str);
}


void PropertyRatioSource::toJson(QJsonObject &obj)
{
    Q_D(const PropertyRatioSource);

    QJsonObject set;

    int total = 0;
    for (auto it = d->ratioSet.constBegin(); it != d->ratioSet.constEnd(); ++it)
        total += it.value();

    for (auto it = d->ratioSet.constBegin(); it != d->ratioSet.constEnd(); ++it)
        set.insert(d->valueToString(it.key()), (double)it.value() / (double)(total));

    obj.insert(d->sampleName, set);
}

void PropertyRatioSource::load(QSettings *settings)
{
    Q_D(PropertyRatioSource);
    settings->beginGroup(QStringLiteral("PropertyRatioSource"));
    settings->beginGroup(d->sampleName);
    const auto count = settings->beginReadArray(QStringLiteral("RatioSet"));

    for (int i = 0; i < count; ++i) {
        settings->setArrayIndex(i);
        const auto value = settings->value(QStringLiteral("Value"));
        const auto amount = settings->value(QStringLiteral("Amount"), 0).toInt();
        d->ratioSet.insert(value, amount);
    }

    settings->endArray();
    settings->endGroup();
    settings->endGroup();
}


void PropertyRatioSource::store(QSettings *settings)
{
    Q_D(const PropertyRatioSource);
    settings->beginGroup(QStringLiteral("PropertyRatioSource"));
    settings->beginGroup(d->sampleName);
    settings->beginWriteArray(QStringLiteral("RatioSet"), d->ratioSet.size());

    int i = 0;
    for (auto it = d->ratioSet.constBegin(); it != d->ratioSet.constEnd(); ++it) {
        settings->setArrayIndex(i++);
        settings->setValue(QStringLiteral("Value"), it.key());
        settings->setValue(QStringLiteral("Amount"), it.value());
    }

    settings->endArray();
    settings->endGroup();
    settings->endGroup();
}

#include "propertyratiosource.moc"
