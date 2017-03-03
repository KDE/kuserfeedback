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
        QTest::qWait(1);
        QCOMPARE(p.surveyInterval(), -1);

        slider->setValue(2);
        QTest::qWait(1);
        QCOMPARE(p.surveyInterval(), -1);
    }
};

QTEST_MAIN(FeedbackConfigTest)

#include "feedbackconfigtest.moc"
