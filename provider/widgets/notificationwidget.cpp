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

#include "notificationwidget.h"
#include "feedbackconfigdialog.h"

#include <provider/core/provider.h>
#include <provider/core/surveyinfo.h>

#include <QAction>
#include <QDebug>
#include <QDesktopServices>

using namespace UserFeedback;

namespace UserFeedback {

class NotificationWidgetPrivate
{
public:
    NotificationWidgetPrivate(NotificationWidget *qq) :
        q(qq),
        provider(nullptr)
    {}

    void clearActions();
    void surveyAvailable(const SurveyInfo &survey);
    void showEncouragement();

    NotificationWidget *q;
    Provider *provider;
};

}

void NotificationWidgetPrivate::clearActions()
{
    qDeleteAll(q->actions());
}

void NotificationWidgetPrivate::surveyAvailable(const SurveyInfo &survey)
{
    Q_ASSERT(provider);
    clearActions();

    q->setText(NotificationWidget::tr("Survey?")); // TODO

    auto surveyAction = new QAction(q);
    surveyAction->setText(NotificationWidget::tr("Participate"));
    QObject::connect(surveyAction, &QAction::triggered, q, [this, survey]() {
        QDesktopServices::openUrl(survey.url());
        provider->setSurveyCompleted(survey);
        q->animatedHide();
    });
    q->addAction(surveyAction);

    q->animatedShow();
}

void NotificationWidgetPrivate::showEncouragement()
{
    Q_ASSERT(provider);
    clearActions();

    q->setText(NotificationWidget::tr("Contribute!")); // TODO

    auto configAction = new QAction(q);
    configAction->setText(NotificationWidget::tr("Contribute..."));
    QObject::connect(configAction, &QAction::triggered, q, [this]() {
        FeedbackConfigDialog dlg(q);
        dlg.setFeedbackProvider(provider);
        dlg.exec();
        q->animatedHide();
    });
    q->addAction(configAction);

    q->animatedShow();
}


NotificationWidget::NotificationWidget(QWidget *parent) :
    KMessageWidget(parent),
    d(new NotificationWidgetPrivate(this))
{
    setVisible(false);
    setMessageType(KMessageWidget::Information);
    setWordWrap(true);
}

NotificationWidget::~NotificationWidget()
{
    delete d;
}

void NotificationWidget::setFeedbackProvider(Provider* provider)
{
    d->provider = provider;
    connect(provider, &Provider::surveyAvailable, this, [this](const SurveyInfo &survey) {
        d->surveyAvailable(survey);
    });
    connect(provider, &Provider::showEncouragementMessage, this, [this]() { d->showEncouragement(); });
}
