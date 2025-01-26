/*
    SPDX-FileCopyrightText: 2016 Volker Krause <vkrause@kde.org>

    SPDX-License-Identifier: MIT
*/

#ifndef KUSERFEEDBACK_CONSOLE_FEEDBACKCONFIGDIALOG_H
#define KUSERFEEDBACK_CONSOLE_FEEDBACKCONFIGDIALOG_H

#include "kuserfeedbackwidgets_export.h"
#include <QDialog>

#include <memory>

namespace KUserFeedback {

class FeedbackConfigDialogPrivate;
class Provider;

/*!
 * \class KUserFeedback::FeedbackConfigDialog
 * \inmodule KUserFeedbackWidgets
 * \inheaderfile KUserFeedback/FeedbackConfigDialog
 *
 * \brief Configure which feedback a user wants to provide.
 *
 * \a FeedbackConfigWidget
 */
class KUSERFEEDBACKWIDGETS_EXPORT FeedbackConfigDialog : public QDialog
{
    Q_OBJECT
public:
    /*!
     * Create a new feedback configuration dialog.
     *
     * \a parent The parent widget.
     */
    explicit FeedbackConfigDialog(QWidget *parent = nullptr);
    ~FeedbackConfigDialog() override;

    /*! Set the feedback provider that this dialog configures. */
    void setFeedbackProvider(KUserFeedback::Provider *provider);

    /*!
     * Accepts the dialog and write changes made by the user to
     * the feedback provider.
     */
    void accept() override;

private:
    std::unique_ptr<FeedbackConfigDialogPrivate> d;
};

}

#endif // KUSERFEEDBACK_CONSOLE_FEEDBACKCONFIGDIALOG_H
