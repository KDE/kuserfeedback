/*
    SPDX-FileCopyrightText: 2017 Volker Krause <vkrause@kde.org>

    SPDX-License-Identifier: MIT
*/

#ifndef KUSERFEEDBACK_CONSOLE_CHARTUTIL_H
#define KUSERFEEDBACK_CONSOLE_CHARTUTIL_H

namespace QtCharts {
class QChart;
}

namespace KUserFeedback {
namespace Console {

namespace ChartUtil
{
void applyTheme(QtCharts::QChart *chart);
}

}}

#endif // KUSERFEEDBACK_CONSOLE_CHARTUTIL_H
