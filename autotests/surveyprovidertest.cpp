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

#include <provider.h>
#include <surveyinfo.h>

#include <QDebug>
#include <QtTest/qtest.h>
#include <QObject>
#include <QSettings>
#include <QSignalSpy>
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
#include <QStandardPaths>
#endif

using namespace KUserFeedback;

class SurveyProviderTest : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase()
    {
        qRegisterMetaType<KUserFeedback::SurveyInfo>();

        QCoreApplication::setOrganizationName(QStringLiteral("KDE"));
        QCoreApplication::setOrganizationDomain(QStringLiteral("kde.org"));
        QCoreApplication::setApplicationName(QStringLiteral("surveyprovidertest"));
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
        QStandardPaths::setTestModeEnabled(true);
#endif
    }

    void testSurveySelect()
    {
        {
            QSettings s(QCoreApplication::organizationName(), QStringLiteral("UserFeedback.org.kde.providertest"));;
            s.beginGroup(QLatin1String("UserFeedback"));
            s.remove(QLatin1String("LastSurvey"));
            s.remove(QLatin1String("CompletedSurveys"));
        }

        SurveyInfo s1;
        s1.setUuid(QUuid::createUuid());
        s1.setUrl(QUrl(QStringLiteral("https://www.kde.org")));
        QVERIFY(s1.isValid());

        Provider p;
        QSignalSpy spy(&p, SIGNAL(surveyAvailable(KUserFeedback::SurveyInfo)));
        QVERIFY(spy.isValid());

        bool rv = false;
        QMetaObject::invokeMethod(&p, "selectSurvey", Q_RETURN_ARG(bool, rv), Q_ARG(KUserFeedback::SurveyInfo, s1));
        QVERIFY(rv);
        QCOMPARE(spy.size(), 1);

        // again, not completed yet
        QMetaObject::invokeMethod(&p, "selectSurvey", Q_RETURN_ARG(bool, rv), Q_ARG(KUserFeedback::SurveyInfo, s1));
        QVERIFY(rv);
        QCOMPARE(spy.size(), 2);
        spy.clear();

        // survey completed, should no longer be accepted
        p.surveyCompleted(s1);
        QMetaObject::invokeMethod(&p, "selectSurvey", Q_RETURN_ARG(bool, rv), Q_ARG(KUserFeedback::SurveyInfo, s1));
        QVERIFY(!rv);
        QVERIFY(spy.isEmpty());
    }
};

QTEST_MAIN(SurveyProviderTest)

#include "surveyprovidertest.moc"
