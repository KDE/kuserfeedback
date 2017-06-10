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

#include "productexportjob.h"

#include <rest/restapi.h>
#include <core/sample.h>

#include <QDir>
#include <QNetworkReply>

using namespace KUserFeedback::Console;

ProductExportJob::ProductExportJob(const QString& productId, const QString& destination, RESTClient* restClient, QObject* parent)
    : Job(parent)
    , m_dest(destination)
    , m_restClient(restClient)
{
    Q_ASSERT(m_restClient);
    Q_ASSERT(m_restClient->isConnected());
    auto reply = RESTApi::listProducts(restClient);
    connect(reply, &QNetworkReply::finished, this, [this, productId, reply]() {
        if (reply->error() != QNetworkReply::NoError) {
            deleteLater();
            return;
        }
        const auto products = Product::fromJson(reply->readAll());
        const auto it = std::find_if(products.begin(), products.end(), [productId](const auto &p) {
            return p.name() == productId;
        });
        if (it == products.end()) {
            emitError(tr("Product not found."));
        } else {
            m_product = *it;
            doExportSchema();
        }
    });
}

ProductExportJob::ProductExportJob(const Product& product, const QString& destination, RESTClient* restClient, QObject* parent)
    : Job(parent)
    , m_product(product)
    , m_dest(destination)
    , m_restClient(restClient)
{
    Q_ASSERT(m_restClient);
    Q_ASSERT(m_restClient->isConnected());
    doExportSchema();
}

ProductExportJob::~ProductExportJob() = default;

void ProductExportJob::doExportSchema()
{
    Q_ASSERT(m_product.isValid());

    QFile f(destination() + QLatin1Char('/') + m_product.name() + QLatin1String(".schema"));
    if (!f.open(QFile::WriteOnly)) {
        emitError(tr("Could not open file: %1").arg(f.errorString()));
        return;
    }
    f.write(m_product.toJson());

    doExportSurveys();
}

void ProductExportJob::doExportSurveys()
{
    auto reply = RESTApi::listSurveys(m_restClient, m_product);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() != QNetworkReply::NoError) {
            deleteLater();
            return;
        }

        QFile f(destination() + QLatin1Char('/') + m_product.name() + QLatin1String(".surveys"));
        if (!f.open(QFile::WriteOnly)) {
            emitError(tr("Could not open file: %1").arg(f.errorString()));
            return;
        }
        f.write(reply->readAll());
        doExportData();
    });
}

void ProductExportJob::doExportData()
{
    auto reply = RESTApi::listSamples(m_restClient, m_product);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() != QNetworkReply::NoError) {
            deleteLater();
            return;
        }

        QFile f(destination() + QLatin1Char('/') + m_product.name() + QLatin1String(".data"));
        if (!f.open(QFile::WriteOnly)) {
            emitError(tr("Could not open file: %1").arg(f.errorString()));
            return;
        }
        const auto samples = Sample::fromJson(reply->readAll(), m_product);
        f.write(Sample::toJson(samples, m_product));
        emitFinished();
    });
}

QString ProductExportJob::destination() const
{
    QDir dest(m_dest);
    Q_ASSERT(dest.exists());

    dest.mkpath(m_product.name());
    dest.cd(m_product.name());
    return dest.absolutePath();
}
