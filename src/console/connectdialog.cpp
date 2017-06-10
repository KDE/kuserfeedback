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

#include <rest/serverinfo.h>

#include <QDebug>
#include <QIcon>
#include <QPushButton>
#include <QUrl>

using namespace KUserFeedback::Console;

ConnectDialog::ConnectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectDialog)
{
    ui->setupUi(this);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Connect"));
    connect(ui->serverName, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ConnectDialog::serverSelected);

    setWindowIcon(QIcon::fromTheme(QStringLiteral("network-connect")));
}

ConnectDialog::~ConnectDialog()
{
}

ServerInfo ConnectDialog::serverInfo() const
{
    ServerInfo info;
    info.setName(ui->serverName->currentText());
    info.setUrl(QUrl(ui->serverUrl->text()));
    info.setUserName(ui->userName->text());
    info.setPassword(ui->password->text());
    return info;
}

void ConnectDialog::setServerInfo(const ServerInfo& serverInfo)
{
    ui->serverName->setCurrentText(serverInfo.name());
    ui->serverUrl->setText(serverInfo.url().toString());
    ui->userName->setText(serverInfo.userName());
    ui->password->setText(serverInfo.password());
}

void ConnectDialog::addRecentServerInfos(const QStringList& serverInfoNames)
{
    ui->serverName->addItems(serverInfoNames);
}

void ConnectDialog::serverSelected()
{
    auto info = ServerInfo::load(ui->serverName->currentText());
    if (info.isValid())
        setServerInfo(info);
}
