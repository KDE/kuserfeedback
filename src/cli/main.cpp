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

#include "config-userfeedback-version.h"

#include <jobs/productexportjob.h>
#include <rest/restapi.h>
#include <rest/restclient.h>
#include <rest/serverinfo.h>
#include <core/product.h>

#include <QCommandLineParser>
#include <QCoreApplication>
#include <QDir>
#include <QNetworkReply>

#include <iostream>

using namespace UserFeedback::Console;

static int jobCount = 0;

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName(QStringLiteral("userfeedbackctl"));
    QCoreApplication::setOrganizationName(QStringLiteral("KDE"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("kde.org"));
    QCoreApplication::setApplicationVersion(QStringLiteral(USERFEEDBACK_VERSION_STRING));

    QCommandLineParser parser;
    parser.setApplicationDescription(QStringLiteral("UserFeedback management tool"));
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption serverOpt({ QStringLiteral("server"), QStringLiteral("s") }, QStringLiteral("Server URL"), QStringLiteral("url"));
    parser.addOption(serverOpt);
    QCommandLineOption outputOpt( { QStringLiteral("output"), QStringLiteral("o") }, QStringLiteral("Output path"), QStringLiteral("path"));
    parser.addOption(outputOpt);
    QCommandLineOption forceOpt( { QStringLiteral("force"), QStringLiteral("f") }, QStringLiteral("Force destructive operations"));
    parser.addOption(forceOpt);
    parser.addPositionalArgument(QStringLiteral("command"), QStringLiteral("Command: delete-product, export-all, export-product, import-product, list-products"));

    parser.process(app);

    if (parser.positionalArguments().isEmpty() || !parser.isSet(serverOpt))
        parser.showHelp(1);

    // TODO: make the server information independent of UserFeedbackConsole
    QCoreApplication::setApplicationName(QStringLiteral("UserFeedbackConsole"));
    const auto server = ServerInfo::load(parser.value(serverOpt));
    QCoreApplication::setApplicationName(QStringLiteral("userfeedbackctl"));

    if (!server.isValid()) {
        std::cerr << "Invalid server information." << std::endl;
        return 1;
    }

    QFileInfo fi(parser.value(outputOpt));
    if (parser.isSet(outputOpt) && (!fi.isDir() || !fi.isWritable())) {
        std::cerr << "Output path does exist or is not accessible." << std::endl;
        return 1;
    }

    RESTClient restClient;
    QObject::connect(&restClient, &RESTClient::errorMessage, [](const auto &msg) {
        std::cerr << qPrintable(msg) << std::endl;
    });
    restClient.setServerInfo(server);

    const auto cmd = parser.positionalArguments().at(0);
    if (cmd == QLatin1String("delete-product")) {
        jobCount = parser.positionalArguments().size() - 1;
        if (jobCount <= 0 || !parser.isSet(forceOpt))
            parser.showHelp(1);
        QObject::connect(&restClient, &RESTClient::clientConnected, [&parser, &restClient]() {
            for (int i = 0; i < jobCount; ++i) {
                Product p;
                p.setName(parser.positionalArguments().at(i+1));
                auto reply = RESTApi::deleteProduct(&restClient, p);
                QObject::connect(reply, &QNetworkReply::finished, [reply]() {
                    if (reply->error() != QNetworkReply::NoError)
                        return;
                    --jobCount;
                    if (jobCount == 0)
                        qApp->quit();
                });
            }
        });
    } else if (cmd == QLatin1String("export-all")) {
        if (parser.positionalArguments().size() != 1)
            parser.showHelp(1);
        QObject::connect(&restClient, &RESTClient::clientConnected, [&]() {
            auto reply = RESTApi::listProducts(&restClient);
            QObject::connect(reply, &QNetworkReply::finished, [reply, &parser, &outputOpt, &restClient]() {
                if (reply->error() != QNetworkReply::NoError)
                    return;
                const auto products = Product::fromJson(reply->readAll());
                for (const auto p : products) {
                    ++jobCount;
                    auto job = new ProductExportJob(p, parser.value(outputOpt), &restClient);
                    QObject::connect(job, &ProductExportJob::destroyed, []() {
                        --jobCount;
                        if (jobCount == 0)
                            qApp->quit();
                    });
                    QObject::connect(job, &ProductExportJob::error, [](const auto &msg) {
                        std::cerr << qPrintable(msg) << std::endl;
                    });
                }
            });
        });
    } else if (cmd == QLatin1String("export-product")) {
        if (parser.positionalArguments().size() != 2)
            parser.showHelp(1);
        QObject::connect(&restClient, &RESTClient::clientConnected, [&parser, &outputOpt, &restClient]() {
            auto job = new ProductExportJob(parser.positionalArguments().at(1), parser.value(outputOpt), &restClient);
            QObject::connect(job, &ProductExportJob::finished, qApp, &QCoreApplication::quit);
            QObject::connect(job, &ProductExportJob::error, [](const auto &msg) {
                std::cerr << qPrintable(msg) << std::endl;
                qApp->quit();
            });
        });
    } else if (cmd == QLatin1String("import-product")) {
        if (parser.positionalArguments().size() <= 1)
            parser.showHelp(1);
        // TODO
        QObject::connect(&restClient, &RESTClient::clientConnected, [&app]() {
            app.quit();
        });
    } else if (cmd == QLatin1String("list-products")) {
        if (parser.positionalArguments().size() != 1)
            parser.showHelp(1);
        QObject::connect(&restClient, &RESTClient::clientConnected, [&restClient]() {
            auto reply = RESTApi::listProducts(&restClient);
            QObject::connect(reply, &QNetworkReply::finished, [reply]() {
                if (reply->error() != QNetworkReply::NoError)
                    return;
                const auto products = Product::fromJson(reply->readAll());
                for (const auto p : products)
                    std::cout << qPrintable(p.name()) << std::endl;
                qApp->quit();
            });
        });
    } else {
        parser.showHelp(1);
    }

    auto reply = RESTApi::checkSchema(&restClient);
    QObject::connect(reply, &QNetworkReply::finished, [reply, &server, &restClient]() {
        if (reply->error() == QNetworkReply::NoError) {
            std::cerr << "Connected to " << qPrintable(server.url().toString()) << std::endl;
            restClient.setConnected(true);
        } else {
            std::cerr << qPrintable(reply->errorString()) << std::endl;
            QCoreApplication::exit(1);
        }
    });

    return app.exec();
}
