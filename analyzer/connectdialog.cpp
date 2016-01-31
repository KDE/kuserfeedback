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

#include "connectdialog.h"
#include "ui_connectdialog.h"

#include "serverinfo.h"

#include <QIcon>
#include <QUrl>

using namespace UserFeedback::Analyzer;

ConnectDialog::ConnectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectDialog)
{
    ui->setupUi(this);

    setWindowIcon(QIcon::fromTheme(QStringLiteral("network-connect")));
}

ConnectDialog::~ConnectDialog()
{
}

ServerInfo ConnectDialog::serverInfo() const
{
    ServerInfo info;
    info.setUrl(QUrl(ui->serverUrl->text()));
    info.setUserName(ui->userName->text());
    info.setPassword(ui->password->text());
    return info;
}

void ConnectDialog::setServerInfo(const ServerInfo& serverInfo)
{
    ui->serverUrl->setText(serverInfo.url().toString());
    ui->userName->setText(serverInfo.userName());
    ui->password->setText(serverInfo.password());
}
