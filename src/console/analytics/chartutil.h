/*
    SPDX-FileCopyrightText: 2017 Volker Krause <vkrause@kde.org>

    SPDX-License-Identifier: MIT
*/

#ifndef KUSERFEEDBACK_CONSOLE_CHARTUTIL_H
#define KUSERFEEDBACK_CONSOLE_CHARTUTIL_H

#include <qglobal.h>

class QChart;

namespace KUserFeedback {
namespace Console {

namespace ChartUtil
{
void applyTheme(QChart *chart);
}

}}

#endif // KUSERFEEDBACK_CONSOLE_CHARTUTIL_H
