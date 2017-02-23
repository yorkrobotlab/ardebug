/* visConfig.cpp
 *
 * This class encapsulates a visualiser configuration.
 *
 * (C) Alistair Jewers Feb 2017
 */

#include "visconfig.h"
#include <QListWidgetItem>

/* Constructor
 * Initialises the element list
 */
VisConfig::VisConfig() {
    this->elements.reserve(5);
}

/* populateSettingsList
 * Fills the settings list widget based on the current configuration.
 */
void VisConfig::populateSettingsList(QListWidget *list) {
    for (size_t i = 0; i < this->elements.size(); i++) {
        VisElement* element = elements.at(i);

        QListWidgetItem* item = new QListWidgetItem;
        item->setData(Qt::DisplayRole, element->toString());
        item->setData(Qt::CheckStateRole, Qt::Checked);
        list->addItem(item);
    }
}
