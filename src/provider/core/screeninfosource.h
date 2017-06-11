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

#ifndef KUSERFEEDBACK_SCREENINFOSOURCE_H
#define KUSERFEEDBACK_SCREENINFOSOURCE_H

#include "kuserfeedbackcore_export.h"
#include "abstractdatasource.h"

namespace KUserFeedback {

/*! Data source for information about connected displays.
 *  This provides as array of maps containing the following properties:
 *  - height: Height of the corresponding screen in pixel.
 *  - width: Width of the corresponding screen in pixel.
 *  - dpi: Dots per inch of the corresponding screen.
 *
 *  The default telemetry mode for this source is Provider::DetailedSystemInformation.
 */
class KUSERFEEDBACKCORE_EXPORT ScreenInfoSource :  public AbstractDataSource
{
    Q_DECLARE_TR_FUNCTIONS(KUserFeedback::ScreenInfoSource)
public:
    /*! Create a new screen information source. */
    ScreenInfoSource();
    QString description() const override;
    QVariant data() override;
};

}

#endif // KUSERFEEDBACK_SCREENINFOSOURCE_H
