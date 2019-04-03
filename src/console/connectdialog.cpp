/*
    Copyright (C) 2016 Volker Krause <vkrause@kde.org>

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
