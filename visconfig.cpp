#include "visconfig.h"

#include <QListWidgetItem>

VisConfig::VisConfig() {
    this->elements.reserve(5);
}

void VisConfig::populateSettingsList(QListWidget *list) {
    for (size_t i = 0; i < this->elements.size(); i++) {
        VisElement* element = elements.at(i);

        QListWidgetItem* item = new QListWidgetItem;
        item->setData(Qt::DisplayRole, element->toString());
        item->setData(Qt::CheckStateRole, Qt::Checked);
        list->addItem(item);
    }
}
