#ifndef USERFEEDBACK_ANALYZER_SCHEMAEDITWIDGET_H
#define USERFEEDBACK_ANALYZER_SCHEMAEDITWIDGET_H

#include <QWidget>

#include <memory>

namespace UserFeedback {
namespace Analyzer {

namespace Ui
{
class SchemaEditWidget;
}

class Product;
class SchemaModel;

class SchemaEditWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SchemaEditWidget(QWidget *parent = nullptr);
    ~SchemaEditWidget();

    void setProduct(const Product &product);

signals:
    void productChanged(const Product &product);

private:
    void addEntry();
    void deleteEntry();

    void updateState();

    std::unique_ptr<Ui::SchemaEditWidget> ui;
    SchemaModel *m_schemaModel;
};

}
}

#endif // USERFEEDBACK_ANALYZER_SCHEMAEDITWIDGET_H
