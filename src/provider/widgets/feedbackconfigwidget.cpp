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

#include "feedbackconfigwidget.h"
#include "ui_feedbackconfigwidget.h"

#include <abstractdatasource.h>
#include <provider.h>

#include <QApplication>
#include <QDebug>
#include <QScrollBar>

#include <algorithm>
#include <vector>

using namespace UserFeedback;

namespace UserFeedback {
class FeedbackConfigWidgetPrivate {
public:
    FeedbackConfigWidgetPrivate();

    int telemetryModeIndex(Provider::StatisticsCollectionMode mode) const;

    void telemetrySliderChanged();
    void surveySliderChanged();
    void applyPalette(QSlider *slider);

    Provider *provider;
    std::unique_ptr<Ui::FeedbackConfigWidget> ui;
    std::vector<Provider::StatisticsCollectionMode> telemetryModeMap;
};

}

FeedbackConfigWidgetPrivate::FeedbackConfigWidgetPrivate()
    : provider(nullptr)
{
}

int FeedbackConfigWidgetPrivate::telemetryModeIndex(Provider::StatisticsCollectionMode mode) const
{
    const auto it = std::lower_bound(telemetryModeMap.begin(), telemetryModeMap.end(), mode);
    if (it == telemetryModeMap.end())
        return 0;
    return std::distance(telemetryModeMap.begin(), it);
}

void FeedbackConfigWidgetPrivate::telemetrySliderChanged()
{
    if (ui->telemetrySlider->value() == 0) {
        ui->telemetryStack->setCurrentWidget(ui->noTelemetryPage);
    } else {
        ui->telemetryStack->setCurrentWidget(ui->telemetryPage);
    }

    switch (telemetryModeMap[ui->telemetrySlider->value()]) {
        case Provider::NoStatistics:
            break;
        case Provider::BasicSystemInformation:
            ui->telemetryLabel->setText(FeedbackConfigWidget::tr(
                "Share basic system information. "
                "No unique identification is included, nor data processed with the application."
            ));
            break;
        case Provider::BasicUsageStatistics:
            ui->telemetryLabel->setText(FeedbackConfigWidget::tr(
                "Share basic system information and basic statistics on how often you use the application. "
                "No unique identification is included, nor data processed with the application."
            ));
            break;
        case Provider::DetailedSystemInformation:
            ui->telemetryLabel->setText(FeedbackConfigWidget::tr(
                "Share basic statistics on how often you use the application, as well as detailed information about your system. "
                "No unique identification is included, nor data processed with the application."
            ));
        case Provider::DetailedUsageStatistics:
            ui->telemetryLabel->setText(FeedbackConfigWidget::tr(
                "Share detailed system information and statistics on how often individual features of the application are used. "
                "No unique identification is included, nor data processed with the application."
            ));
            break;
    }

    applyPalette(ui->telemetrySlider);

    if (!provider)
        return;

    if (!ui->rawTelemetryButton->isChecked()) {
        auto detailsStr = QStringLiteral("<ul>");
        foreach (const auto *src, provider->dataSources()) {
            if (ui->telemetrySlider->value() >= telemetryModeIndex(src->collectionMode()))
                detailsStr += QStringLiteral("<li>") + src->description() + QStringLiteral("</li>");
        }
        ui->telemetryDetails->setHtml(detailsStr + QStringLiteral("</ul>"));
    } else {
        QByteArray jsonData;
        QMetaObject::invokeMethod(provider, "jsonData", Q_RETURN_ARG(QByteArray, jsonData), Q_ARG(UserFeedback::Provider::StatisticsCollectionMode, telemetryModeMap[ui->telemetrySlider->value()]));
        ui->telemetryDetails->setPlainText(QString::fromUtf8(jsonData.constData()));
    }
}

void FeedbackConfigWidgetPrivate::surveySliderChanged()
{
    if (!provider)
        return;

    switch (ui->surveySlider->value()) {
        case 0:
            ui->surveyLabel->setText(FeedbackConfigWidget::tr(
                "We make this application for you. In order to ensure it actually does what you need it to do we "
                "would like to ask you about your use cases and your feedback, in the form of a web survey."
            ));
            break;
        case 1:
            ui->surveyLabel->setText(FeedbackConfigWidget::tr(
                "I will occasionally participate in web surveys about the application, not more than four times a year though."
            ));
            break;
        case 2:
            ui->surveyLabel->setText(FeedbackConfigWidget::tr(
                "I will participate in web surveys whenever one is available. Surveys can of course be defered or skipped."
            ));
            break;
    }

    applyPalette(ui->surveySlider);
}

void FeedbackConfigWidgetPrivate::applyPalette(QSlider* slider)
{
    const auto ratio = (double)slider->value() / (double)slider->maximum();
    const auto red = qBound<double>(0.0, 2.0 - ratio * 2.0, 1.0);
    const auto green = qBound<double>(0.0, ratio * 2.0, 1.0);
    auto color = QColor(255 * red, 255 * green, 0);

    if (QApplication::palette().color(QPalette::Base).lightness() > 128)
        color = color.lighter(150);
    else
        color = color.darker(150);

    auto pal = slider->palette();
    pal.setColor(QPalette::Highlight, color);
    slider->setPalette(pal);
}


FeedbackConfigWidget::FeedbackConfigWidget(QWidget* parent)
    : QWidget(parent)
    , d(new FeedbackConfigWidgetPrivate)
{
    d->ui.reset(new Ui::FeedbackConfigWidget);
    d->ui->setupUi(this);

    connect(d->ui->telemetrySlider, SIGNAL(valueChanged(int)), this, SLOT(telemetrySliderChanged()));
    connect(d->ui->telemetrySlider, SIGNAL(valueChanged(int)), this, SIGNAL(configurationChanged()));
    connect(d->ui->surveySlider, SIGNAL(valueChanged(int)), this, SLOT(surveySliderChanged()));
    connect(d->ui->surveySlider, SIGNAL(valueChanged(int)), this, SIGNAL(configurationChanged()));

    d->ui->rawTelemetryButton->setParent(d->ui->telemetryDetails);
    d->ui->rawTelemetryButton->setIcon(style()->standardPixmap(QStyle::SP_DialogHelpButton));
    d->ui->telemetryDetails->installEventFilter(this);
    connect(d->ui->rawTelemetryButton, SIGNAL(toggled(bool)), this, SLOT(telemetrySliderChanged()));

    setEnabled(false); // see setFeedbackProvider
}

FeedbackConfigWidget::~FeedbackConfigWidget()
{
}

Provider* FeedbackConfigWidget::feedbackProvider() const
{
    return d->provider;
}

void FeedbackConfigWidget::setFeedbackProvider(Provider* provider)
{
    d->provider = provider;
    if (!provider) {
        setEnabled(false);
        return;
    }

    d->telemetryModeMap.clear();
    d->telemetryModeMap.reserve(5);
    d->telemetryModeMap.push_back(Provider::NoStatistics);
    d->telemetryModeMap.push_back(Provider::BasicSystemInformation);
    d->telemetryModeMap.push_back(Provider::BasicUsageStatistics);
    d->telemetryModeMap.push_back(Provider::DetailedSystemInformation);
    d->telemetryModeMap.push_back(Provider::DetailedUsageStatistics);

    QSet<Provider::StatisticsCollectionMode> supportedModes;
    supportedModes.reserve(d->telemetryModeMap.size());
    supportedModes.insert(Provider::NoStatistics);
    foreach (const auto &src, provider->dataSources())
        supportedModes.insert(src->collectionMode());
    for (auto it = d->telemetryModeMap.begin(); it != d->telemetryModeMap.end();) {
        if (!supportedModes.contains(*it))
            it = d->telemetryModeMap.erase(it);
        else
            ++it;
    }

    const auto hasTelemetry = d->telemetryModeMap.size() > 1;
    d->ui->telemetrySlider->setEnabled(hasTelemetry);
    d->ui->telemetryStack->setEnabled(hasTelemetry);
    if (hasTelemetry)
        d->ui->telemetrySlider->setMaximum(d->telemetryModeMap.size() - 1);

    d->ui->telemetrySlider->setValue(d->telemetryModeIndex(provider->statisticsCollectionMode()));

    if (provider->surveyInterval() < 0)
        d->ui->surveySlider->setValue(0);
    else if (provider->surveyInterval() >= 90)
        d->ui->surveySlider->setValue(1);
    else
        d->ui->surveySlider->setValue(2);
    d->surveySliderChanged(); // update the description even if nothing changed initially

    setEnabled(provider);
}

bool FeedbackConfigWidget::eventFilter(QObject* receiver, QEvent* event)
{
    if (receiver == d->ui->telemetryDetails) {
        d->ui->rawTelemetryButton->move(
            d->ui->telemetryDetails->width()
                - d->ui->rawTelemetryButton->width()
                - style()->pixelMetric(QStyle::PM_LayoutRightMargin)
                - (d->ui->telemetryDetails->verticalScrollBar()->isVisible() ? d->ui->telemetryDetails->verticalScrollBar()->width() : 0),
            d->ui->telemetryDetails->height()
                - d->ui->rawTelemetryButton->height()
                - style()->pixelMetric(QStyle::PM_LayoutBottomMargin)
        );
    }
    return QWidget::eventFilter(receiver, event);
}

Provider::StatisticsCollectionMode FeedbackConfigWidget::statisticsCollectionMode() const
{
    return d->telemetryModeMap[d->ui->telemetrySlider->value()];
}

int FeedbackConfigWidget::surveyInterval() const
{
    switch (d->ui->surveySlider->value()) {
        case 0: return -1;
        case 1: return 90;
        case 2: return 0;
    }
    return -1;
}

#include "moc_feedbackconfigwidget.cpp"
