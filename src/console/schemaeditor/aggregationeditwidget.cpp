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

#include <QMenu>
#include <QStyledItemDelegate>

using namespace UserFeedback::Console;

AggregationEditWidget::AggregationEditWidget(QWidget* parent) :
    QWidget(parent),
    ui(new Ui::AggregationEditWidget),
    m_model(new AggregationEditorModel(this)),
    m_editorFactory(new SchemaEntryItemEditorFactory)
{
    ui->setupUi(this);

    ui->aggregationView->setModel(m_model);
    qobject_cast<QStyledItemDelegate*>(ui->aggregationView->itemDelegate())->setItemEditorFactory(m_editorFactory.get());
    connect(ui->aggregationView, &QWidget::customContextMenuRequested, this, &AggregationEditWidget::contextMenu);

    connect(ui->actionAddAggregation, &QAction::triggered, this, &AggregationEditWidget::addAggregation);
    connect(ui->actionDeleteAggregation, &QAction::triggered, this, &AggregationEditWidget::deleteAggregation);

    addActions({ ui->actionAddAggregation, ui->actionDeleteAggregation });
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
}

void AggregationEditWidget::addAggregation()
{
    auto p = product();
    auto aggrs = p.aggregations();
    aggrs += Aggregation();
    p.setAggregations(aggrs);
    setProduct(p);
}

void AggregationEditWidget::deleteAggregation()
{
    const auto rows = ui->aggregationView->selectionModel()->selectedRows();
    if (rows.isEmpty())
        return;

    const auto idx = rows.at(0);
    auto p = product();
    auto aggrs = p.aggregations();
    aggrs.remove(idx.row());
    p.setAggregations(aggrs);
    setProduct(p);
}

void AggregationEditWidget::updateState()
{
    ui->actionAddAggregation->setEnabled(product().isValid());
    ui->actionDeleteAggregation->setEnabled(ui->aggregationView->selectionModel()->hasSelection());
}

void AggregationEditWidget::contextMenu(QPoint pos)
{
    QMenu menu;
    menu.addActions({ ui->actionAddAggregation, ui->actionDeleteAggregation });
    menu.exec(ui->aggregationView->viewport()->mapToGlobal(pos));
}
