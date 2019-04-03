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

#ifndef KUSERFEEDBACK_STYLEINFOSOURCE_H
#define KUSERFEEDBACK_STYLEINFOSOURCE_H

#include "kuserfeedbackwidgets_export.h"

#include "abstractdatasource.h"

namespace KUserFeedback {

/*! Data source the widget style and color scheme used by the application. */
class KUSERFEEDBACKWIDGETS_EXPORT StyleInfoSource : public AbstractDataSource
{
public:
    Q_DECLARE_TR_FUNCTIONS(KUserFeedback::StyleInfoSource)
public:
    StyleInfoSource();

    QString name() const override;
    QString description() const override;

    QVariant data() override;
};

}

#endif // KUSERFEEDBACK_STYLEINFOSOURCE_H
