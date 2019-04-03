/*
    Copyright (C) 2016 Volker Krause <vkrause@kde.org>

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

#include "restapi.h"
#include "restclient.h"

#include <core/product.h>
#include <core/sample.h>
#include <core/survey.h>

#include <QUuid>

using namespace KUserFeedback::Console;

QNetworkReply* RESTApi::checkSchema(RESTClient *client)
{
    return client->get(QStringLiteral("analytics/check_schema"));
}

QNetworkReply* RESTApi::listProducts(RESTClient *client)
{
    return client->get(QStringLiteral("analytics/products"));
}

QNetworkReply* RESTApi::createProduct(RESTClient *client, const Product &p)
{
    Q_ASSERT(p.isValid());
    return client->post(QStringLiteral("admin/products"),  p.toJson());
}

QNetworkReply* RESTApi::updateProduct(RESTClient *client, const Product &p)
{
    Q_ASSERT(p.isValid());
    return client->put(QStringLiteral("admin/products/") + p.name(), p.toJson());
}

QNetworkReply* RESTApi::deleteProduct(RESTClient *client, const Product &p)
{
    Q_ASSERT(p.isValid());
    return client->deleteResource(QStringLiteral("admin/products/") + p.name());
}

QNetworkReply* RESTApi::listSamples(RESTClient *client, const Product &p)
{
    Q_ASSERT(p.isValid());
    return client->get(QStringLiteral("analytics/data/") + p.name());
}

QNetworkReply* RESTApi::addSamples(RESTClient *client, const Product &p, const QVector<Sample> &samples)
{
    Q_ASSERT(p.isValid());
    return client->post(QStringLiteral("admin/data/") + p.name(), Sample::toJson(samples, p));
}

QNetworkReply* RESTApi::listSurveys(RESTClient *client, const Product &p)
{
    Q_ASSERT(p.isValid());
    return client->get(QStringLiteral("analytics/surveys/") + p.name());
}

QNetworkReply* RESTApi::createSurvey(RESTClient *client, const Product &p, const Survey &s)
{
    Q_ASSERT(p.isValid());
    return client->post(QStringLiteral("admin/surveys/") + p.name(), s.toJson());
}

QNetworkReply* RESTApi::updateSurvey(RESTClient *client, const Survey &s)
{
    return client->put(QStringLiteral("admin/surveys/") + s.uuid().toString(), s.toJson());
}

QNetworkReply* RESTApi::deleteSurvey(RESTClient *client, const Survey &s)
{
    return client->deleteResource(QStringLiteral("admin/surveys/") + s.uuid().toString());
}
