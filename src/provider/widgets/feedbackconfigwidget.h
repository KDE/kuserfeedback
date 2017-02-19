/*
    Copyright (C) 2017 Volker Krause <vkrause@kde.org>

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

#ifndef USERFEEDBACK_FEEDBACKCONFIGWIDGET_H
#define USERFEEDBACK_FEEDBACKCONFIGWIDGET_H

#include "userfeedbackwidgets_export.h"

#include <QWidget>

#include <memory>

namespace UserFeedback {

class FeedbackConfigWidgetPrivate;
class Provider;

/**
 * Configuration widget for telemetry and survey contributions.
 *
 * @see FeedbackConfigDialog
 */
class USERFEEDBACKWIDGETS_EXPORT FeedbackConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FeedbackConfigWidget(QWidget *parent = nullptr);
    ~FeedbackConfigWidget();

    /** Returns the feedback provider configured by this widget. */
    Provider* feedbackProvier() const;

    /** Set the feedback provider that should be configured with this widget. */
    void setFeedbackProvider(Provider *provider);

protected:
    bool eventFilter(QObject *receiver, QEvent *event) override;

private:
    Q_PRIVATE_SLOT(d, void telemetrySliderChanged())
    Q_PRIVATE_SLOT(d, void surveySliderChanged())
    std::unique_ptr<FeedbackConfigWidgetPrivate> d;
};

}

#endif // USERFEEDBACK_FEEDBACKCONFIGWIDGET_H
