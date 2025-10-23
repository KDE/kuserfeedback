/*
    SPDX-FileCopyrightText: 2017 Aleix Pol Gonzalez <aleixpol@kde.org>

    SPDX-License-Identifier: MIT
*/

#ifndef KUSERFEEDBACK_QMLPROPERTYSOURCE_H
#define KUSERFEEDBACK_QMLPROPERTYSOURCE_H

#include "qmlabstractdatasource.h"
#include <QVariant>

class CustomPropertySource;

namespace KUserFeedback {

/*!
 * \qmltype PropertySource
 * \inqmlmodule org.kde.userfeedback
 * \inherits AbstractDataSource
 *
 * \brief Data source reporting information provided by a QML application.
 */
class QmlPropertySource : public QmlAbstractDataSource
{
    Q_OBJECT

    /*!
     * \qmlproperty string PropertySource::sourceId
     */
    Q_PROPERTY(QString sourceId READ sourceId WRITE setSourceId NOTIFY idChanged)

    /*!
     * \qmlproperty string PropertySource::name
     */
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

    /*!
     * \qmlproperty string PropertySource::description
     */
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)

    /*!
     * \qmlproperty variant PropertySource::data
     */
    Q_PROPERTY(QVariant data READ data WRITE setData NOTIFY dataChanged)
public:
    explicit QmlPropertySource(QObject* parent = nullptr);

    QString sourceId() const;
    QString name() const;
    QString description() const;
    QVariant data() const;

    void setSourceId(const QString &id);
    void setName(const QString &name);
    void setDescription(const QString &description);
    void setData(const QVariant &data);

Q_SIGNALS:
    void idChanged(const QString &id);
    void nameChanged(const QString &name);
    void descriptionChanged(const QString &description);
    void dataChanged(const QVariant &data);

private:
    CustomPropertySource* customSource();
};
}

#endif // KUSERFEEDBACK_QTVERSIONSOURCE_H
