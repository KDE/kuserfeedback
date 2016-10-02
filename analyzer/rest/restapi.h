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

#ifndef USERFEEDBACK_ANALYZER_RESTAPI_H
#define USERFEEDBACK_ANALYZER_RESTAPI_H

class QNetworkReply;

namespace UserFeedback {
namespace Analyzer {

class Product;
class RESTClient;

/** C++ wrapper for the server-side API.
 *  Precondition for all methods is that client->isConnected() returns @c true.
 */
namespace RESTApi
{
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
}

}
}

#endif // USERFEEDBACK_ANALYZER_RESTAPI_H
