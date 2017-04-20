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

#ifndef USERFEEDBACK_STYLEINFOSOURCE_H
#define USERFEEDBACK_STYLEINFOSOURCE_H

#include "userfeedbackwidgets_export.h"

#include <provider/core/abstractdatasource.h>

namespace UserFeedback {

/*! Data source the widget style and color scheme used by the application. */
class USERFEEDBACKWIDGETS_EXPORT StyleInfoSource : public AbstractDataSource
{
public:
    Q_DECLARE_TR_FUNCTIONS(UserFeedback::StyleInfoSource)
public:
    StyleInfoSource();
    QString description() const override;
    QVariant data() override;
};

}

#endif // USERFEEDBACK_STYLEINFOSOURCE_H
