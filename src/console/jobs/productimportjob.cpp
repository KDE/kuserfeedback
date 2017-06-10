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

#include "productimportjob.h"

#include <rest/restapi.h>
#include <rest/restclient.h>
#include <core/product.h>
#include <core/sample.h>
#include <core/survey.h>

#include <QDir>
#include <QNetworkReply>

using namespace KUserFeedback::Console;

ProductImportJob::ProductImportJob(const QString& source, RESTClient* restClient, QObject* parent)
    : Job(parent)
    , m_source(source)
    , m_restClient(restClient)
{
    Q_ASSERT(m_restClient->isConnected());
    doImportSchema();
}

ProductImportJob::~ProductImportJob() = default;

void ProductImportJob::doImportSchema()
{
    QDir source(m_source);
    Q_ASSERT(source.exists());

    QFile f(source.absoluteFilePath(source.dirName() + QLatin1String(".schema")));
    if (!f.open(QFile::ReadOnly)) {
        emitError(tr("Unable to open file: %1").arg(f.errorString()));
        return;
    }

    const auto products = Product::fromJson(f.readAll());
    if (products.size() != 1) {
        emitError(tr("Invalid product schema file."));
        return;
    }
    m_product = products.at(0);

    auto reply = RESTApi::createProduct(m_restClient, m_product);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() != QNetworkReply::NoError) {
            deleteLater();
            return;
        }
        doImportSurveys();
    });
}

void ProductImportJob::doImportSurveys()
{
    QDir source(m_source);
    QFile f(source.absoluteFilePath(source.dirName() + QLatin1String(".surveys")));
    if (!f.open(QFile::ReadOnly)) {
        doImportData();
        return;
    }

    const auto surveys = Survey::fromJson(f.readAll());
    if (surveys.isEmpty()) {
        doImportData();
        return;
    }

    for (const auto &s : surveys) {
        ++m_jobCount;
        auto reply = RESTApi::createSurvey(m_restClient, m_product, s);
        connect(reply, &QNetworkReply::finished, this, [this, reply]() {
            --m_jobCount;
            if (reply->error() != QNetworkReply::NoError) {
                deleteLater();
                return;
            }
            if (m_jobCount == 0)
                doImportData();
        });
    }
}

void ProductImportJob::doImportData()
{
    QDir source(m_source);
    QFile f(source.absoluteFilePath(source.dirName() + QLatin1String(".data")));
    if (!f.open(QFile::ReadOnly)) {
        emitFinished();
        return;
    }

    const auto samples = Sample::fromJson(f.readAll(), m_product);
    if (samples.isEmpty()) {
        emitFinished();
        return;
    }

    auto reply = RESTApi::addSamples(m_restClient, m_product, samples);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError)
            emitFinished();
    });
}
