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

#ifndef USERFEEDBACK_ANALYZER_AGGREGATIONEDITWIDGET_H
#define USERFEEDBACK_ANALYZER_AGGREGATIONEDITWIDGET_H

#include <QWidget>

#include <memory>

namespace UserFeedback {
namespace Analyzer {

class AggregationEditorModel;
class Product;
class SchemaEntryItemEditorFactory;

namespace Ui
{
class AggregationEditWidget;
}

class AggregationEditWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AggregationEditWidget(QWidget *parent = nullptr);
    ~AggregationEditWidget();

    Product product() const;
    void setProduct(const Product &product);

private:
    void addAggregation();
    void deleteAggregation();

    void updateState();

    std::unique_ptr<Ui::AggregationEditWidget> ui;
    AggregationEditorModel *m_model;
    std::unique_ptr<SchemaEntryItemEditorFactory> m_editorFactory;
};
}
}

#endif // USERFEEDBACK_ANALYZER_AGGREGATIONEDITWIDGET_H
