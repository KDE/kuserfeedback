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
