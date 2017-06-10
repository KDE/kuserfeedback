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

#ifndef KUSERFEEDBACK_CONSOLE_HELPCONTROLLER_H
#define KUSERFEEDBACK_CONSOLE_HELPCONTROLLER_H

class QString;

namespace KUserFeedback {
namespace Console {

/*! Controls the Assistant-based help browser. */
namespace HelpController
{

/*! Returns @c true if Assistant and our help collection are found. */
bool isAvailable();

/*! Open start page of the help collection. */
void openContents();

/*! Opens the specified page of the help collection. */
void openPage(const QString &page);

}

}}

#endif // KUSERFEEDBACK_CONSOLE_HELPCONTROLLER_H
