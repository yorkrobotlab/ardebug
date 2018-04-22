/* viscustom.cpp
 *
 * This class encapsulates the visualisation of robot path.
 *
 * (C) Alistair Jewers Feb 2017
 */

#include "vispath.h"
#include "../Core/settings.h"

/* Constructor
 * Initialise all setttings
 */
VisPath::VisPath() {
    setType(VisType::PATH);
    setEnabled(false);
    setSelectedOnly(true);
    settingsDialog = NULL;
}

/* Destructor
 * Release all memory
 */
VisPath::~VisPath() {
    if (settingsDialog != NULL) {
        delete settingsDialog;
    }
}

/* toString
 * Generate a string describing all settings.
 */
QString VisPath::toString(void) {
    QString str = "Path. ";

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
void VisPath::render(QWidget* widget, QPainter* painter, RobotData *robot, bool selected) {

}

/* getSettingsDialog
 * Return a pointer to the settings dialog for this visualisation.
 */
QDialog* VisPath::getSettingsDialog(void) {
    if (settingsDialog != NULL) {
        delete settingsDialog;
    }

    settingsDialog = new PathSettingsDialog(this);
    return settingsDialog;
}

/* setSelectedOnly
 * Enables or disables the selected robot only setting
 */
void VisPath::setSelectedOnly(bool enable) {
    selectedOnly = enable;
}

/* getSelectedOnly
 * Return the current state of the selected robot only setting
 */
bool VisPath::getSelectedOnly(void) {
    return selectedOnly;
}
