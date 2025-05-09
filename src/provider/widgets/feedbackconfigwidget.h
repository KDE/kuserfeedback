/*
    SPDX-FileCopyrightText: 2017 Volker Krause <vkrause@kde.org>

    SPDX-License-Identifier: MIT
*/

#ifndef KUSERFEEDBACK_FEEDBACKCONFIGWIDGET_H
#define KUSERFEEDBACK_FEEDBACKCONFIGWIDGET_H

#include "kuserfeedbackwidgets_export.h"

#include <KUserFeedback/Provider>

#include <QWidget>

#include <memory>

namespace KUserFeedback {

class FeedbackConfigWidgetPrivate;
class Provider;

/*!
 * \class KUserFeedback::FeedbackConfigWidget
 * \inmodule KUserFeedbackWidgets
 * \inheaderfile KUserFeedback/FeedbackConfigWidget
 *
 * \brief Configuration widget for telemetry and survey contributions.
 *
 * Use this rather than FeedbackConfigDialog if you want to embed the
 * feedback configuration for example into an existing configuration
 * dialog.
 *
 * \sa FeedbackConfigDialog
 */
class KUSERFEEDBACKWIDGETS_EXPORT FeedbackConfigWidget : public QWidget
{
    Q_OBJECT
public:
    /*!
     * Create a new feedback provider configuration widget.
     *
     * \a parent The parent widget.
     */
    explicit FeedbackConfigWidget(QWidget *parent = nullptr);
    ~FeedbackConfigWidget() override;

    /*! Returns the feedback provider configured by this widget. */
    Provider* feedbackProvider() const;

    /*! Set the feedback provider that should be configured with this widget. */
    void setFeedbackProvider(Provider *provider);

    /*! Returns the telemetry level currently selected in the widget. */
    Provider::TelemetryMode telemetryMode() const;

    /*! Returns the survey interval currently selected in this widget. */
    int surveyInterval() const;

Q_SIGNALS:
    /*!
     * Emitted when any changes are made to the configuration represented
     * in this widget.
     */
    void configurationChanged();

protected:
    bool eventFilter(QObject *receiver, QEvent *event) override;

private:
    std::unique_ptr<FeedbackConfigWidgetPrivate> d;
};

}

#endif // KUSERFEEDBACK_FEEDBACKCONFIGWIDGET_H
