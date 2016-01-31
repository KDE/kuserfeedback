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

#ifndef USERFEEDBACK_PROVIDER_H
#define USERFEEDBACK_PROVIDER_H

#include "userfeedbackcore_export.h"

#include <QObject>
#include <QUrl>


namespace UserFeedback {

class ProviderPrivate;

/** The central object managing data sources and transmitting feedback to the server. */
class USERFEEDBACKCORE_EXPORT Provider : public QObject
{
    Q_OBJECT
public:
    explicit Provider(QObject *parent = Q_NULLPTR);

    /** Set the product identifier.
     *  This is used to distinguish independent products on the same server.
     */
    void setProductIdentifier(const QString &productId);

    /** Set the feedback server URL. */
    void setFeedbackServer(const QUrl &url);

public slots:
    /** Manually submit currently recorded data. */
    void submit();

private:
    friend class ProviderPrivate;
    ProviderPrivate * const d;
    Q_PRIVATE_SLOT(d, void aboutToQuit());
    Q_PRIVATE_SLOT(d, void submitFinished());
};

}

#endif // USERFEEDBACK_PROVIDER_H
