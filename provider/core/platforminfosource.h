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

#ifndef USERFEEDBACK_PLATFORMINFOSOURCE_H
#define USERFEEDBACK_PLATFORMINFOSOURCE_H

#include "abstractdatasource.h"

namespace UserFeedback {

/** Reports information about the platform the application is running on.
 *  This includes two string data fields:
 *  - platformOS: the operating system name
 *  - platformVersion: the operating system or distribution version.
 */
class USERFEEDBACKCORE_EXPORT PlatformInfoSource : public AbstractDataSource
{
public:
    PlatformInfoSource();
    void toJson(QJsonObject &obj) Q_DECL_OVERRIDE;
};

}

#endif // USERFEEDBACK_PLATFORMINFOSOURCE_H
