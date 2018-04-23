#ifndef VISNAME_H
#define VISNAME_H

#include "viselement.h"
#include "../UI/namesettingsdialog.h"

class VisName : public VisElement
{
public:
    VisName();

    virtual QString toString(void);

    virtual void render(QWidget* widget, QPainter* painter, RobotData *robot, bool selected, QRectF rect);

    virtual QDialog* getSettingsDialog(void);

    void setSelectedOnly(bool enable);
    bool getSelectedOnly(void);

private:
    NameSettingsDialog* settingsDialog;

    bool selectedOnly;
};

#endif // VISNAME_H
