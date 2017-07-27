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

#ifndef KUSERFEEDBACK_QMLDATASOURCES_H
#define KUSERFEEDBACK_QMLDATASOURCES_H

#include "qmlabstractdatasource.h"

namespace KUserFeedback {

#define MAKE_BASIC_QML_WRAPPER(x) \
    class Qml ## x : public QmlAbstractDataSource { \
        Q_OBJECT \
    public: \
        explicit Qml ## x (QObject *parent = nullptr);\
    };

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

#undef MAKE_BASIC_QML_WRAPPER
}

#endif // KUSERFEEDBACK_QMLDATASOURCES_H
