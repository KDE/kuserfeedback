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

#include "chartexportdialog.h"
#include "ui_chartexportdialog.h"

#include <QFileDialog>

using namespace KUserFeedback::Console;

ChartExportDialog::ChartExportDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ChartExportDialog)
{
    ui->setupUi(this);

    connect(ui->fileEdit, &QLineEdit::textChanged, this, &ChartExportDialog::validate);
    connect(ui->fileButton, &QPushButton::clicked, this, &ChartExportDialog::fileButtonClicked);
    validate();
}

ChartExportDialog::~ChartExportDialog() = default;

ChartExportDialog::Type ChartExportDialog::type() const
{
    if (ui->imageButton->isChecked())
        return Image;
    if (ui->svgButton->isChecked())
        return SVG;
    if (ui->pdfButton->isChecked())
        return PDF;
    Q_UNREACHABLE();
}

QString ChartExportDialog::filename() const
{
    return ui->fileEdit->text();
}

QSize ChartExportDialog::size() const
{
    return QSize(ui->width->value(), ui->height->value());
}

void ChartExportDialog::fileButtonClicked()
{
    const auto fn = QFileDialog::getSaveFileName(this, tr("Export Chart"));
    if (!fn.isEmpty())
        ui->fileEdit->setText(fn);
}

void ChartExportDialog::validate()
{
    ui->buttonBox->button(QDialogButtonBox::Save)->setEnabled(!ui->fileEdit->text().isEmpty());
}
