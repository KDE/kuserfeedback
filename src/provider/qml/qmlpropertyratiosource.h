/*
    SPDX-FileCopyrightText: 2017 Volker Krause <vkrause@kde.org>

    SPDX-License-Identifier: MIT
*/

#ifndef KUSERFEEDBACK_QMLPROPERTYRATIOSOURCE_H
#define KUSERFEEDBACK_QMLPROPERTYRATIOSOURCE_H

#include "qmlabstractdatasource.h"

namespace KUserFeedback {

class PropertyRatioSource;

/*!
 * \qmltype PropertyRatioSource
 * \inqmlmodule org.kde.userfeedback
 * \nativetype KUserFeedback::PropertyRatioSource
 * \inherits AbstractDataSource
 */
class QmlPropertyRatioSource : public QmlAbstractDataSource
{
    Q_OBJECT

    /*!
     * \qmlproperty string PropertyRatioSource::sourceId
     */
    Q_PROPERTY(QString sourceId READ sourceId WRITE setSourceId NOTIFY changed)

    /*!
     * \qmlproperty string PropertyRatioSource::name
     */
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY changed)

    /*!
     * \qmlproperty string PropertyRatioSource::description
     */
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY changed)

    /*!
     * \qmlproperty QtObject PropertyRatioSource::object
     */
    Q_PROPERTY(QObject* object READ object WRITE setObject NOTIFY changed)

    /*!
     * \qmlproperty string PropertyRatioSource::propertyName
     */
    Q_PROPERTY(QString propertyName READ propertyName WRITE setPropertyName NOTIFY changed)
public:
    explicit QmlPropertyRatioSource(QObject *parent = nullptr);
    ~QmlPropertyRatioSource() override;

    QString sourceId() const;
    void setSourceId(const QString &id);

    QString name() const;
    void setName(const QString &name);

    QString description() const;
    void setDescription(const QString &desc);

    QObject* object() const;
    void setObject(QObject *object);

    QString propertyName() const;
    void setPropertyName(const QString &name);

    /*!
     * \qmlmethod PropertyRatioSource::addValueMapping(variant value, string name)
     */
    Q_INVOKABLE void addValueMapping(const QVariant &value, const QString &name);

Q_SIGNALS:
    void changed();

private:
    PropertyRatioSource* prSrc() const;
};

}

#endif // KUSERFEEDBACK_QMLPROPERTYRATIOSOURCE_H
