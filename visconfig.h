#ifndef VISCONFIG_H
#define VISCONFIG_H

#include <vector>
#include <viselement.h>

#include <QListWidget>

class VisConfig
{
public:
    std::vector<VisElement*> elements;

    VisConfig();

    void populateSettingsList(QListWidget* list);
};

#endif // VISCONFIG_H
