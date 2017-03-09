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

#include <provider/core/provider.h>
#include <provider/core/platforminfosource.h>
#include <provider/core/screeninfosource.h>

#include <QDebug>
#include <QtTest/qtest.h>
#include <QObject>
#include <QSettings>
#include <QSignalSpy>
#include <QStandardPaths>

using namespace UserFeedback;

class ProviderTest : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase()
    {
        QStandardPaths::setTestModeEnabled(true);
    }

    void testNoTelemetry()
    {
        Provider provider;
        provider.setProductIdentifier(QStringLiteral("org.kde.UserFeedback.UnitTestProduct"));
        provider.addDataSource(new ScreenInfoSource, Provider::BasicSystemInformation);
        provider.addDataSource(new PlatformInfoSource, Provider::BasicSystemInformation);
        provider.setStatisticsCollectionMode(Provider::NoStatistics);
        QByteArray b;
        QMetaObject::invokeMethod(&provider, "jsonData", Q_RETURN_ARG(QByteArray, b), Q_ARG(UserFeedback::Provider::StatisticsCollectionMode, provider.statisticsCollectionMode()));
        b.replace('\n', "");
        QCOMPARE(b.constData(), "{}");
    }

    void testLoadStore()
    {
        {
            Provider p;
            p.setStatisticsCollectionMode(Provider::NoStatistics);
            p.setSurveyInterval(-1);
        }

        {
            Provider p;
            QCOMPARE(p.statisticsCollectionMode(), Provider::NoStatistics);
            QCOMPARE(p.surveyInterval(), -1);
            p.setStatisticsCollectionMode(Provider::DetailedSystemInformation);
            p.setSurveyInterval(90);
        }

        {
            Provider p;
            QCOMPARE(p.statisticsCollectionMode(), Provider::DetailedSystemInformation);
            QCOMPARE(p.surveyInterval(), 90);
        }
    }

    void testEncouragement()
    {
        {
            QSettings s;
            s.beginGroup(QLatin1String("UserFeedback"));
            s.remove(QLatin1String("LastEncouragement"));
        }

        {
            Provider p;
            QSignalSpy spy(&p, SIGNAL(showEncouragementMessage()));
            QVERIFY(spy.isValid());
            p.setEncouragementDelay(0);
            QVERIFY(!spy.wait(10));
            p.setApplicationStartsUntilEncouragement(0);
            p.setApplicationUsageTimeUntilEncouragement(0);
            QVERIFY(spy.wait(10));
            QCOMPARE(spy.count(), 1);
        }

        {
            Provider p;
            QSignalSpy spy(&p, SIGNAL(showEncouragementMessage()));
            QVERIFY(spy.isValid());
            p.setEncouragementDelay(0);
            p.setApplicationStartsUntilEncouragement(0);
            p.setApplicationUsageTimeUntilEncouragement(0);
            QVERIFY(!spy.wait(10));
        }
    }

    void testEncouragementDelay()
    {
        {
            QSettings s;
            s.beginGroup(QLatin1String("UserFeedback"));
            s.remove(QLatin1String("LastEncouragement"));
        }

        {
            Provider p;
            QSignalSpy spy(&p, SIGNAL(showEncouragementMessage()));
            QVERIFY(spy.isValid());
            p.setEncouragementDelay(1);
            p.setApplicationStartsUntilEncouragement(0);
            p.setApplicationUsageTimeUntilEncouragement(0);
            QVERIFY(!spy.wait(10));
            QVERIFY(spy.wait(1200));
            QCOMPARE(spy.count(), 1);
        }
    }

    void testNoEncouragementWithAllFeedbackEnabled()
    {
        {
            QSettings s;
            s.beginGroup(QLatin1String("UserFeedback"));
            s.remove(QLatin1String("LastEncouragement"));
        }

        {
            Provider p;
            p.addDataSource(new PlatformInfoSource, Provider::BasicSystemInformation);
            p.setSurveyInterval(0);
            p.setStatisticsCollectionMode(Provider::BasicSystemInformation);
            QSignalSpy spy(&p, SIGNAL(showEncouragementMessage()));
            QVERIFY(spy.isValid());
            p.setEncouragementDelay(0);
            p.setApplicationStartsUntilEncouragement(0);
            p.setApplicationUsageTimeUntilEncouragement(0);
            QVERIFY(!spy.wait(10));
            QCOMPARE(spy.count(), 0);
        }

    }

    void testEncouragementRepetition()
    {
        {
            QSettings s;
            s.beginGroup(QLatin1String("UserFeedback"));
            s.setValue(QLatin1String("LastEncouragement"), QDateTime::currentDateTime().addSecs(-24 * 60 * 60 + 1));
        }

        {
            Provider p;
            p.setSurveyInterval(-1);
            p.setStatisticsCollectionMode(Provider::NoStatistics);
            QSignalSpy spy(&p, SIGNAL(showEncouragementMessage()));
            QVERIFY(spy.isValid());
            p.setEncouragementDelay(0);
            p.setApplicationStartsUntilEncouragement(0);
            p.setEncouragementInterval(1);
            QVERIFY(spy.wait(1000));
            QCOMPARE(spy.count(), 1);
        }

        {
            QSettings s;
            s.beginGroup(QLatin1String("UserFeedback"));
            s.setValue(QLatin1String("LastEncouragement"), QDateTime::currentDateTime().addSecs(-24 * 60 * 60 - 1));
        }

        {
            Provider p;
            p.setSurveyInterval(90);
            p.setStatisticsCollectionMode(Provider::BasicSystemInformation);
            QSignalSpy spy(&p, SIGNAL(showEncouragementMessage()));
            QVERIFY(spy.isValid());
            p.setEncouragementDelay(0);
            p.setApplicationStartsUntilEncouragement(0);
            p.setEncouragementInterval(1);
            QVERIFY(!spy.wait(10));
        }
    }
};

QTEST_MAIN(ProviderTest)

#include "providertest.moc"
