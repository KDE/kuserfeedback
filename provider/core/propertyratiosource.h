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

#ifndef USERFEEDBACK_PROPERTYRATIOSOURCE_H
#define USERFEEDBACK_PROPERTYRATIOSOURCE_H

#include "userfeedbackcore_export.h"
#include "abstractdatasource.h"

namespace UserFeedback {

class PropertyRatioSourcePrivate;

/** Records the time ratio a given QObject property has a specific value.
 *
 *  An example use-case would be the usage ratio of a applications
 *  views/modes selected by a QTabWidget or QRadioButton.
 */
class USERFEEDBACKCORE_EXPORT PropertyRatioSource :  public AbstractDataSource
{
public:
    /** Create a new property ratio data source.
     * @param obj the QObject of which a property should be monitored.
     * @param propertyName The name of the property to monitor.
     * This property must have a change notification signal. The value must have
     * comparison operators registerd.
     * @param sampleName This is the name of the database field this data source is
     * associated with.
     */
    explicit PropertyRatioSource(QObject *obj, const char* propertyName, const QString &sampleName);

    /** Map property value @value to @str for sending to the server.
     *  This is useful to map internal identifiers to portable and persistable values,
     *  such as turning pointers or indexes into meaningful descriptions.
     */
    void addValueMapping(const QVariant &value, const QString &str);

    void toJson(QJsonObject &obj) Q_DECL_OVERRIDE;
    void load(QSettings *settings) Q_DECL_OVERRIDE;
    void store(QSettings *settings) Q_DECL_OVERRIDE;

private:
    Q_DECLARE_PRIVATE(PropertyRatioSource)
};

}

#endif // USERFEEDBACK_PROPERTYRATIOSOURCE_H
