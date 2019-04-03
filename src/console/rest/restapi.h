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

#ifndef KUSERFEEDBACK_CONSOLE_RESTAPI_H
#define KUSERFEEDBACK_CONSOLE_RESTAPI_H

class QNetworkReply;
template <typename T> class QVector;

namespace KUserFeedback {
namespace Console {

class Product;
class RESTClient;
class Sample;
class Survey;

/** C++ wrapper for the server-side API.
 *  Precondition for all methods is that client->isConnected() returns @c true.
 */
namespace RESTApi
{
    /** Check if server database schema is up-to-date. */
    QNetworkReply* checkSchema(RESTClient *client);

    /** List all products. */
    QNetworkReply* listProducts(RESTClient *client);

    /** Add a new product.
     *  @param p The product to add. Must be valid.
     */
    QNetworkReply* createProduct(RESTClient *client, const Product &p);

    /** Update an existing product.
     *  @param p The product to add. Must be valid.
     */
    QNetworkReply* updateProduct(RESTClient *client, const Product &p);

    /** Deletes a product.
     *  @param p The product to add. Must be valid.
     */
    QNetworkReply* deleteProduct(RESTClient *client, const Product &p);

    /** Get all samples for a product.
     *  @param p The product to add. Must be valid.
     */
    QNetworkReply* listSamples(RESTClient *client, const Product &p);

    /** Add the given samples to a product.
     *  @param p The product to add. Must be valid.
     */
    QNetworkReply* addSamples(RESTClient *client, const Product &p, const QVector<Sample> &samples);

    /** List all surveys for product @p p. */
    QNetworkReply* listSurveys(RESTClient *client, const Product &p);

    /** Create a new survey. */
    QNetworkReply* createSurvey(RESTClient *client, const Product &p, const Survey &s);

    /** Update an existing survey. */
    QNetworkReply* updateSurvey(RESTClient *client, const Survey &s);

    /** Delete a survey. */
    QNetworkReply* deleteSurvey(RESTClient *client, const Survey &s);
}

}
}

#endif // KUSERFEEDBACK_CONSOLE_RESTAPI_H
