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

#include "qmldatasources.h"

#include <ApplicationVersionSource>
#include <CompilerInfoSource>
#include <CpuInfoSource>
#include <LocaleInfoSource>
#include <OpenGLInfoSource>
#include <PlatformInfoSource>
#include <QPAInfoSource>
#include <QtVersionSource>
#include <ScreenInfoSource>
#include <StartCountSource>
#include <UsageTimeSource>

using namespace KUserFeedback;

#define MAKE_BASIC_QML_WRAPPER(x) Qml ## x::Qml ## x(QObject *parent) : QmlAbstractDataSource(new x, parent) {}

MAKE_BASIC_QML_WRAPPER(ApplicationVersionSource)
MAKE_BASIC_QML_WRAPPER(CompilerInfoSource)
MAKE_BASIC_QML_WRAPPER(CpuInfoSource)
MAKE_BASIC_QML_WRAPPER(LocaleInfoSource)
MAKE_BASIC_QML_WRAPPER(OpenGLInfoSource)
MAKE_BASIC_QML_WRAPPER(PlatformInfoSource)
MAKE_BASIC_QML_WRAPPER(QPAInfoSource)
MAKE_BASIC_QML_WRAPPER(QtVersionSource)
MAKE_BASIC_QML_WRAPPER(ScreenInfoSource)
MAKE_BASIC_QML_WRAPPER(StartCountSource)
MAKE_BASIC_QML_WRAPPER(UsageTimeSource)

#undef MAKE_DEFAULT_SOURCE
