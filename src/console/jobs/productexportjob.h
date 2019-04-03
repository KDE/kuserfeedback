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

#ifndef KUSERFEEDBACK_CONSOLE_PRODUCTEXPORTJOB_H
#define KUSERFEEDBACK_CONSOLE_PRODUCTEXPORTJOB_H

#include "job.h"
#include <rest/restclient.h>
#include <core/product.h>

namespace KUserFeedback {
namespace Console {

class ProductExportJob : public Job
{
    Q_OBJECT
public:
    explicit ProductExportJob(const QString &productId, const QString &destination, RESTClient *restClient, QObject *parent = nullptr);
    explicit ProductExportJob(const Product &product, const QString &destination, RESTClient *restClient, QObject *parent = nullptr);
    ~ProductExportJob();

private:
    void doExportSchema();
    void doExportSurveys();
    void doExportData();

    QString destination() const;

    Product m_product;
    QString m_dest;
    RESTClient *m_restClient;
};

}
}

#endif // KUSERFEEDBACK_CONSOLE_PRODUCTEXPORTJOB_H
