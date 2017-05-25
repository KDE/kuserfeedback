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

#include <provider.h>
#include <platforminfosource.h>
#include <screeninfosource.h>
#include <startcountsource.h>

#include <QDebug>
#include <QtTest/qtest.h>
#include <QObject>
#include <QSettings>
#include <QSignalSpy>
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QStandardPaths>
#endif

using namespace UserFeedback;

class ProviderTest : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase()
    {
        QCoreApplication::setOrganizationName(QStringLiteral("KDE"));
        QCoreApplication::setOrganizationDomain(QStringLiteral("kde.org"));
        QCoreApplication::setApplicationName(QStringLiteral("providertest"));
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        QStandardPaths::setTestModeEnabled(true);
#endif
    }

    void testProductId()
    {
        Provider p;
        QCOMPARE(p.productIdentifier(), QLatin1String("org.kde.providertest"));
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
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        {
            QSettings s(QCoreApplication::organizationName(), QStringLiteral("UserFeedback.org.kde.providertest"));;
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
#endif
    }

    void testEncouragementDelay()
    {
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        {
            QSettings s(QCoreApplication::organizationName(), QStringLiteral("UserFeedback.org.kde.providertest"));;
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
#endif
    }

    void testNoEncouragementWithAllFeedbackEnabled()
    {
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        {
            QSettings s(QCoreApplication::organizationName(), QStringLiteral("UserFeedback.org.kde.providertest"));;
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
#endif
    }

    void testEncouragementRepetition()
    {
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        {
            QSettings s(QCoreApplication::organizationName(), QStringLiteral("UserFeedback.org.kde.providertest"));;
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
            QSettings s(QCoreApplication::organizationName(), QStringLiteral("UserFeedback.org.kde.providertest"));;
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
#endif
    }

    void testMultipleProviders()
    {
        {
            Provider p0;
            p0.setStatisticsCollectionMode(Provider::BasicUsageStatistics); // triggers store, so we want to avoid that below
        }

        int c1 = -1;
        {
            Provider p1;
            auto s1 = new StartCountSource;
            p1.addDataSource(s1, Provider:: BasicUsageStatistics);
            c1 = s1->data().toMap().value(QLatin1String("value")).toInt();
            int c2 = -1;

            {
                Provider p2;
                auto s2 = new StartCountSource;
                p2.addDataSource(s2, Provider:: BasicUsageStatistics);
                c2 = s2->data().toMap().value(QLatin1String("value")).toInt();
            }

            QVERIFY(c2 == c1 + 1);
        }

        Provider p3;
        auto s3 = new StartCountSource;
        p3.addDataSource(s3, Provider:: BasicUsageStatistics);
        int c3 = s3->data().toMap().value(QLatin1String("value")).toInt();
        QVERIFY(c3 == c1 + 2);
    }

    void testProductIdChange()
    {
        {
            Provider p0;
        }

        int c1 = -1;
        {
            Provider p1;
            auto s1 = new StartCountSource;
            p1.addDataSource(s1, Provider::BasicUsageStatistics);
            p1.setSurveyInterval(90);
            const auto c0 = s1->data().toMap().value(QLatin1String("value")).toInt();

            p1.setProductIdentifier(QStringLiteral("org.kde.some.other.test"));
            p1.setSurveyInterval(-1);
            p1.setSurveyInterval(1);
            c1 = s1->data().toMap().value(QLatin1String("value")).toInt();
            QVERIFY(c0 != c1);
        }

        {
            Provider p2;
            auto s2 = new StartCountSource;
            p2.addDataSource(s2, Provider::BasicUsageStatistics);
            p2.setProductIdentifier(QStringLiteral("org.kde.some.other.test"));
            QCOMPARE(p2.surveyInterval(), 1);
            const auto c2 = s2->data().toMap().value(QLatin1String("value")).toInt();
            QCOMPARE(c1 + 1, c2);
        }
    }

};

QTEST_MAIN(ProviderTest)

#include "providertest.moc"
