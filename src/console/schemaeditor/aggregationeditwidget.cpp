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

#include "aggregationeditwidget.h"
#include "ui_aggregationeditwidget.h"
#include "schemaentryitemeditorfactory.h"

#include <core/aggregation.h>
#include <model/aggregationeditormodel.h>
#include <model/aggregationelementeditmodel.h>

#include <QMenu>
#include <QMessageBox>
#include <QStyledItemDelegate>

using namespace KUserFeedback::Console;

AggregationEditWidget::AggregationEditWidget(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::AggregationEditWidget),
    m_model(new AggregationEditorModel(this)),
    m_editorFactory(new SchemaEntryItemEditorFactory),
    m_elementModel(new AggregationElementEditModel(this))
{
    ui->setupUi(this);

    ui->aggregationView->setModel(m_model);
    ui->aggregationView->header()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    qobject_cast<QStyledItemDelegate*>(ui->aggregationView->itemDelegate())->setItemEditorFactory(m_editorFactory.get());
    connect(ui->aggregationView, &QWidget::customContextMenuRequested, this, &AggregationEditWidget::contextMenu);
    connect(ui->aggregationView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &AggregationEditWidget::selectionChanged);
    connect(m_model, &QAbstractItemModel::dataChanged, this, &AggregationEditWidget::productChanged);
    connect(m_model, &QAbstractItemModel::dataChanged, this, &AggregationEditWidget::updateState);

    connect(ui->actionAddAggregation, &QAction::triggered, this, &AggregationEditWidget::addAggregation);
    connect(ui->actionDeleteAggregation, &QAction::triggered, this, &AggregationEditWidget::deleteAggregation);

    ui->elementView->setModel(m_elementModel);
    ui->elementView->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    qobject_cast<QStyledItemDelegate*>(ui->elementView->itemDelegate())->setItemEditorFactory(m_editorFactory.get());
    connect(ui->elementView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &AggregationEditWidget::updateState);
    connect(m_elementModel, &QAbstractItemModel::dataChanged, this, [this]() {
        auto p = product();
        auto aggrs = p.aggregations();
        aggrs[ui->aggregationView->selectionModel()->selectedRows().at(0).row()] = m_elementModel->aggregation();
        p.setAggregations(aggrs);
        m_model->setProduct(p);
        emit productChanged();
    });
    connect(ui->addButton, &QPushButton::clicked, this, &AggregationEditWidget::addElement);
    connect(ui->deleteButton, &QPushButton::clicked, this, &AggregationEditWidget::deleteElement);

    addActions({ ui->actionAddAggregation, ui->actionDeleteAggregation });
    updateState();
}

AggregationEditWidget::~AggregationEditWidget() = default;

Product AggregationEditWidget::product() const
{
    return m_model->product();
}

void AggregationEditWidget::setProduct(const Product& product)
{
    m_model->setProduct(product);
    m_editorFactory->setProduct(product);
    updateState();
}

Aggregation AggregationEditWidget::currentAggregation() const
{
    const auto rows = ui->aggregationView->selectionModel()->selectedRows();
    if (rows.isEmpty())
        return {};

    return product().aggregations().at(rows.at(0).row());
}

void AggregationEditWidget::addAggregation()
{
    auto p = product();
    auto aggrs = p.aggregations();
    aggrs += Aggregation();
    p.setAggregations(aggrs);
    setProduct(p);
    emit productChanged();
}

void AggregationEditWidget::deleteAggregation()
{
    const auto rows = ui->aggregationView->selectionModel()->selectedRows();
    if (rows.isEmpty())
        return;

    const auto r = QMessageBox::critical(this, tr("Delete Aggregation"),
        tr("Do you want to delete the currently selected aggregation?"),
        QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Discard);
    if (r != QMessageBox::Discard)
        return;

    const auto idx = rows.at(0);
    auto p = product();
    auto aggrs = p.aggregations();
    aggrs.remove(idx.row());
    p.setAggregations(aggrs);
    setProduct(p);
    emit productChanged();
}

void AggregationEditWidget::addElement()
{
    auto aggr = m_elementModel->aggregation();
    auto elems = aggr.elements();
    elems.push_back({});
    aggr.setElements(elems);
    m_elementModel->setAggregation(aggr);
}

void AggregationEditWidget::deleteElement()
{
    auto aggr = m_elementModel->aggregation();
    auto elems = aggr.elements();
    elems.removeAt(ui->elementView->selectionModel()->selectedRows().at(0).row());
    aggr.setElements(elems);
    m_elementModel->setAggregation(aggr);

    auto p = product();
    auto aggrs = p.aggregations();
    aggrs[ui->aggregationView->selectionModel()->selectedRows().at(0).row()] = aggr;
    p.setAggregations(aggrs);
    m_model->setProduct(p);
    emit productChanged();
}

void AggregationEditWidget::updateState()
{
    ui->actionAddAggregation->setEnabled(product().isValid());
    ui->actionDeleteAggregation->setEnabled(ui->aggregationView->selectionModel()->hasSelection());

    const auto aggr = currentAggregation();
    const auto isMultiElement = aggr.type() == Aggregation::Category;
    ui->elementView->setEnabled(isMultiElement);
    ui->addButton->setEnabled(isMultiElement);
    ui->deleteButton->setEnabled(isMultiElement && ui->elementView->selectionModel()->hasSelection());
}

void AggregationEditWidget::contextMenu(QPoint pos)
{
    QMenu menu;
    menu.addActions({ ui->actionAddAggregation, ui->actionDeleteAggregation });
    menu.exec(ui->aggregationView->viewport()->mapToGlobal(pos));
}

void AggregationEditWidget::selectionChanged()
{
    m_elementModel->setAggregation(currentAggregation());
    updateState();
}
