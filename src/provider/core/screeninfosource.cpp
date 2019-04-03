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

#include "screeninfosource.h"

#include <QGuiApplication>
#include <QScreen>
#include <QVariant>

using namespace KUserFeedback;

ScreenInfoSource::ScreenInfoSource() :
    AbstractDataSource(QStringLiteral("screens"), Provider::DetailedSystemInformation)
{
}

QString ScreenInfoSource::description() const
{
    return tr("Size and resolution of all connected screens.");
}

QVariant ScreenInfoSource::data()
{
    QVariantList l;
    foreach (auto screen, QGuiApplication::screens()) {
        QVariantMap m;
        m.insert(QStringLiteral("width"), screen->size().width());
        m.insert(QStringLiteral("height"), screen->size().height());
        m.insert(QStringLiteral("dpi"), qRound(screen->physicalDotsPerInch()));
        l.push_back(m);
    }
    return l;
}

QString ScreenInfoSource::name() const
{
    return tr("Screen parameters");
}
