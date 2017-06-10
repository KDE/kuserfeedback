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

#ifndef KUSERFEEDBACK_CONSOLE_CONNECTDIALOG_H
#define KUSERFEEDBACK_CONSOLE_CONNECTDIALOG_H

#include <QDialog>

#include <memory>

namespace KUserFeedback {
namespace Console {

namespace Ui
{
class ConnectDialog;
}

class ServerInfo;

class ConnectDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ConnectDialog(QWidget *parent = nullptr);
    ~ConnectDialog();

    ServerInfo serverInfo() const;
    void setServerInfo(const ServerInfo &serverInfo);

    void addRecentServerInfos(const QStringList &serverInfoNames);

private:
    void serverSelected();

    std::unique_ptr<Ui::ConnectDialog> ui;
};

}
}

#endif // KUSERFEEDBACK_CONSOLE_CONNECTDIALOG_H
