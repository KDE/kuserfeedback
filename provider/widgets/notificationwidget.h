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

#ifndef USERFEEDBACK_NOTIFICATIONWIDGET_H
#define USERFEEDBACK_NOTIFICATIONWIDGET_H

#include "userfeedbackwidgets_export.h"
#include <KMessageWidget>

namespace UserFeedback {

class NotificationWidgetPrivate;
class Provider;

/** Embeddable notification widget to inform the user about surveys and
 *  encourage contribution.
 */
class USERFEEDBACKWIDGETS_EXPORT NotificationWidget : public KMessageWidget
{
    Q_OBJECT
public:
    explicit NotificationWidget(QWidget *parent = nullptr);
    ~NotificationWidget();

    void setFeedbackProvider(Provider *provider);

private:
    friend class NotificationWidgetPrivate;
    NotificationWidgetPrivate * const d;
};

}

#endif // USERFEEDBACK_NOTIFICATIONWIDGET_H
