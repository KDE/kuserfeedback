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

#include "schemaeditwidget.h"
#include "ui_schemaeditwidget.h"
#include "schemaentryitemeditorfactory.h"

#include <model/schemamodel.h>
#include <rest/restapi.h>
#include <core/product.h>
#include <core/schemaentrytemplates.h>

#include <QDebug>
#include <QFileDialog>
#include <QMenu>
#include <QMessageBox>
#include <QNetworkReply>
#include <QStyledItemDelegate>

using namespace UserFeedback::Analyzer;

SchemaEditWidget::SchemaEditWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SchemaEditWidget),
    m_schemaModel(new SchemaModel(this))
{
    ui->setupUi(this);

    ui->schemaView->setModel(m_schemaModel);
    qobject_cast<QStyledItemDelegate*>(ui->schemaView->itemDelegate())->setItemEditorFactory(new SchemaEntryItemEditorFactory);

    connect(ui->addEntryButton, &QPushButton::clicked, this, &SchemaEditWidget::addEntry);
    connect(ui->newEntryName, &QLineEdit::returnPressed, this, &SchemaEditWidget::addEntry);
    connect(ui->actionDelete, &QAction::triggered, this, &SchemaEditWidget::deleteEntry);
    connect(ui->actionSave, &QAction::triggered, this, &SchemaEditWidget::save);
    connect(ui->actionImportSchema, &QAction::triggered, this, &SchemaEditWidget::importSchema);
    connect(ui->actionExportSchema, &QAction::triggered, this, &SchemaEditWidget::exportSchema);

    connect(ui->schemaView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &SchemaEditWidget::updateState);
    connect(ui->newEntryName, &QLineEdit::textChanged, this, &SchemaEditWidget::updateState);

    addActions({ ui->actionDelete, ui->actionSave, ui->actionImportSchema, ui->actionExportSchema });
    updateState();
}

SchemaEditWidget::~SchemaEditWidget() = default;

void SchemaEditWidget::setRESTClient(RESTClient* client)
{
    m_restClient = client;
}

Product SchemaEditWidget::product() const
{
    return m_schemaModel->product();
}

void SchemaEditWidget::setProduct(const Product& product)
{
    m_schemaModel->setProduct(product);
    updateState();
}

void SchemaEditWidget::addEntry()
{
    m_schemaModel->addEntry(ui->newEntryName->text());
    ui->newEntryName->clear();
}

void SchemaEditWidget::deleteEntry()
{
    const auto selection = ui->schemaView->selectionModel()->selection();
    if (selection.isEmpty())
        return;

    const auto r = QMessageBox::critical(this, tr("Delete Schema Entry"), tr("Do you really want to delete this entry, and all recorded data for it?"),
                                         QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Cancel);
    if (r != QMessageBox::Discard)
        return;

    const auto idx = selection.first().topLeft();
    m_schemaModel->deleteRow(idx);
}

void SchemaEditWidget::save()
{
    auto reply = RESTApi::updateProduct(m_restClient, m_schemaModel->product());
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() != QNetworkReply::NoError)
            return;
        emit logMessage(QString::fromUtf8((reply->readAll())));
        emit productChanged(m_schemaModel->product());
    });
}

void SchemaEditWidget::updateState()
{
    const auto selection = ui->schemaView->selectionModel()->selection();
    ui->actionDelete->setEnabled(!selection.isEmpty());

    ui->addEntryButton->setEnabled(!ui->newEntryName->text().isEmpty());

    ui->actionSave->setEnabled(m_schemaModel->product().isValid());
}

void SchemaEditWidget::exportSchema()
{
    const auto fileName = QFileDialog::getSaveFileName(this, tr("Export Schema"));
    if (fileName.isEmpty())
        return;

    QFile f(fileName);
    if (!f.open(QFile::WriteOnly)) {
        QMessageBox::critical(this, tr("Export Failed"), tr("Could not open file: %1").arg(f.errorString()));
        return;
    }
    f.write(m_schemaModel->product().toJson());
    logMessage(tr("Schema of %1 exported to %2.").arg(m_schemaModel->product().name(), f.fileName()));
}

void SchemaEditWidget::importSchema()
{
    const auto fileName = QFileDialog::getOpenFileName(this, tr("Import Schema"));
    if (fileName.isEmpty())
        return;

    QFile f(fileName);
    if (!f.open(QFile::ReadOnly)) {
        QMessageBox::critical(this, tr("Import Failed"), tr("Could not open file: %1").arg(f.errorString()));
        return;
    }
    const auto products = Product::fromJson(f.readAll());
    if (products.size() != 1 || !products.at(0).isValid()) {
        QMessageBox::critical(this, tr("Import Failed"), tr("Selected file contains no valid product schema."));
        return;
    }

    auto p = products.at(0);
    p.setName(m_schemaModel->product().name());
    m_schemaModel->setProduct(p);
    logMessage(tr("Schema of %1 imported from %2.").arg(m_schemaModel->product().name(), f.fileName()));
}
