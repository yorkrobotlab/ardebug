/* viscustom.cpp
 *
 * This class encapsulates the visualisation of robot id.
 *
 * (C) Alistair Jewers Feb 2017
 */

#include "visid.h"
#include "../Core/settings.h"

/* Constructor
 * Initialise all setttings
 */
VisID::VisID(void) {
    setType(VisType::ID);
    setEnabled(true);
    setSelectedOnly(false);
    settingsDialog = NULL;
}

/* Destructor
 * Release all memory
 */
VisID::~VisID() {
    if (settingsDialog != NULL) {
        delete settingsDialog;
    }
}

/* toString
 * Generate a string describing all settings.
 */
QString VisID::toString(void) {
    QString str = "ID. ";

    if (selectedOnly) {
        str = str + "\t\t[Selected robot only. ]";
    } else {
        str = str + "\t\t[All robots. ]";
    }

    return str;
}

/* render
 * Render this visualisation for one robot.
 */
void VisID::render(QWidget* widget, QPainter* painter, RobotData *robot, bool selected, QRectF rect) {
}

/* getSettingsDialog
 * Return a pointer to the settings dialog for this visualisation.
 */
QDialog* VisID::getSettingsDialog(void) {
    if (settingsDialog != NULL) {
        delete settingsDialog;
    }

    settingsDialog = new IDSettingsDialog(this);
    return settingsDialog;
}

/* setSelectedOnly
 * Enables or disables the selected robot only setting
 */
void VisID::setSelectedOnly(bool enable) {
    selectedOnly = enable;
}

/* getSelectedOnly
 * Return the current state of the selected robot only setting
 */
bool VisID::getSelectedOnly(void) {
    return selectedOnly;
}
