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

#ifndef KUSERFEEDBACK_LOCALEINFOSOURCE_H
#define KUSERFEEDBACK_LOCALEINFOSOURCE_H

#include "kuserfeedbackcore_export.h"
#include "abstractdatasource.h"

namespace KUserFeedback {

/*! Data source reporting the region and language settings.
 *
 *  The default telemetry mode for this source is Provider::DetailedSystemInformation.
 */
class KUSERFEEDBACKCORE_EXPORT LocaleInfoSource : public AbstractDataSource
{
    Q_DECLARE_TR_FUNCTIONS(KUserFeedback::LocaleInfoSource)
public:
    LocaleInfoSource();
    QString description() const override;
    QVariant data() override;
};

}

#endif // KUSERFEEDBACK_LOCALEINFOSOURCE_H
