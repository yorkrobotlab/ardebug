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
    elements.reserve(5);
}

VisConfig::~VisConfig(void) {
    for (size_t i = 0; i < elements.size(); i++) {
        delete elements[i];
    }
    elements.clear();
}

/* populateSettingsList
 * Fills the settings list widget based on the current configuration.
 */
void VisConfig::populateSettingsList(QListWidget *list) {
    list->clear();

    for (size_t i = 0; i < elements.size(); i++) {
        VisElement* element = elements.at(i);

        QListWidgetItem* item = new QListWidgetItem;
        item->setData(Qt::DisplayRole, element->toString());
        item->setData(Qt::CheckStateRole, Qt::Checked);
        item->setData(Qt::UserRole, QVariant::fromValue<VisElement*>(element));
        list->addItem(item);
    }
}
