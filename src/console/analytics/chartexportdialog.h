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

#ifndef KUSERFEEDBACK_CONSOLE_CHARTEXPORTDIALOG_H
#define KUSERFEEDBACK_CONSOLE_CHARTEXPORTDIALOG_H

#include <QDialog>

#include <memory>

namespace KUserFeedback {
namespace Console {

namespace Ui
{
class ChartExportDialog;
}

class ChartExportDialog : public QDialog
{
    Q_OBJECT
public:
    enum Type { Image, SVG, PDF };

    explicit ChartExportDialog(QWidget *parent);
    ~ChartExportDialog();

    Type type() const;
    QString filename() const;
    QSize size() const;

private:
    void fileButtonClicked();
    void validate();

    std::unique_ptr<Ui::ChartExportDialog> ui;
};

}}

#endif // KUSERFEEDBACK_CONSOLE_CHARTEXPORTDIALOG_H
