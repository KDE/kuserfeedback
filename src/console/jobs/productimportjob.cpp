/*
    Copyright (C) 2017 Volker Krause <vkrause@kde.org>

    Permission is hereby granted, free of charge, to any person obtaining
    a copy of this software and associated documentation files (the
    "Software"), to deal in the Software without restriction, including
    without limitation the rights to use, copy, modify, merge, publish,
    distribute, sublicense, and/or sell copies of the Software, and to
    permit persons to whom the Software is furnished to do so, subject to
    the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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
        reply->deleteLater();
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
            reply->deleteLater();
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
        reply->deleteLater();
        if (reply->error() == QNetworkReply::NoError)
            emitFinished();
    });
}
