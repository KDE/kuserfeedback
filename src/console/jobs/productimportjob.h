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

#ifndef USERFEEDBACK_CONSOLE_PRODUCTIMPORTJOB_H
#define USERFEEDBACK_CONSOLE_PRODUCTIMPORTJOB_H

#include "job.h"

#include <core/product.h>

namespace UserFeedback {
namespace Console {

class RESTClient;

class ProductImportJob : public Job
{
    Q_OBJECT
public:
    explicit ProductImportJob(const QString &source, RESTClient *restClient, QObject *parent = nullptr);
    ~ProductImportJob();

private:
    void doImportSchema();
    void doImportSurveys();
    void doImportData();

    Product m_product;
    QString m_source;
    RESTClient *m_restClient;
    int m_jobCount = 0;
};

}
}

#endif // USERFEEDBACK_CONSOLE_PRODUCTIMPORTJOB_H
