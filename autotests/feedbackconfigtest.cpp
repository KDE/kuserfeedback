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

#include <provider/widgets/feedbackconfigwidget.h>
#include <provider/core/platforminfosource.h>
#include <provider/core/provider.h>
#include <provider/core/screeninfosource.h>

#include <QDebug>
#include <QtTest/qtest.h>
#include <QObject>
#include <QSlider>
#include <QStandardPaths>

using namespace UserFeedback;

class FeedbackConfigTest : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase()
    {
        QStandardPaths::setTestModeEnabled(true);
    }

    void testTelemetrySettings()
    {
        Provider p;
        p.setProductIdentifier(QStringLiteral("org.kde.UserFeedback.UnitTestProduct"));
        p.addDataSource(new ScreenInfoSource, Provider::BasicSystemInformation);
        p.addDataSource(new PlatformInfoSource, Provider::DetailedSystemInformation);
        p.setStatisticsCollectionMode(Provider::NoStatistics);

        FeedbackConfigWidget w;
        w.setFeedbackProvider(&p);
        w.show();

        auto telemetrySlider = w.findChild<QSlider*>(QLatin1String("telemetrySlider"));
        QVERIFY(telemetrySlider);
        QVERIFY(telemetrySlider->isEnabled());
        QCOMPARE(telemetrySlider->minimum(), 0);
        QCOMPARE(telemetrySlider->maximum(), 2);
        QCOMPARE(telemetrySlider->value(), 0);

        telemetrySlider->setValue(1);
        QCOMPARE(w.statisticsCollectionMode(), Provider::BasicSystemInformation);
        QTest::qWait(1);
        QCOMPARE(p.statisticsCollectionMode(), Provider::NoStatistics);
    }

    void testSurveySettings()
    {
        Provider p;
        p.setProductIdentifier(QStringLiteral("org.kde.UserFeedback.UnitTestProduct"));
        p.setSurveyInterval(-1);

        FeedbackConfigWidget w;
        w.setFeedbackProvider(&p);
        w.show();

        auto slider = w.findChild<QSlider*>(QLatin1String("surveySlider"));
        QVERIFY(slider);
        QVERIFY(slider->isEnabled());
        QCOMPARE(slider->minimum(), 0);
        QCOMPARE(slider->maximum(), 2);
        QCOMPARE(slider->value(), 0);

        slider->setValue(1);
        QCOMPARE(w.surveyInterval(), 90);
        QTest::qWait(1);
        QCOMPARE(p.surveyInterval(), -1);

        slider->setValue(2);
        QCOMPARE(w.surveyInterval(), 0);
        QTest::qWait(1);
        QCOMPARE(p.surveyInterval(), -1);
    }
};

QTEST_MAIN(FeedbackConfigTest)

#include "feedbackconfigtest.moc"
