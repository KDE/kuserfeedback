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

#include "schemaeditor.h"
#include "ui_schemaeditor.h"

using namespace UserFeedback::Analyzer;

SchemaEditor::SchemaEditor(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::SchemaEditor)
{
    ui->setupUi(this);

    connect(ui->schema, &SchemaEditWidget::logMessage, this, &SchemaEditor::logMessage);
    connect(ui->schema, &SchemaEditWidget::productChanged, this, &SchemaEditor::productChanged);

    addActions(ui->schema->actions());
    addActions(ui->aggregation->actions());
}

SchemaEditor::~SchemaEditor() = default;

void SchemaEditor::setRESTClient(RESTClient* client)
{
    ui->schema->setRESTClient(client);
}

void SchemaEditor::setProduct(const Product& product)
{
    ui->schema->setProduct(product);
    ui->aggregation->setProduct(product);
}
