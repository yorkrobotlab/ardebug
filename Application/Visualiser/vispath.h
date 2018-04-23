#ifndef VISPATH_H
#define VISPATH_H

#include "viselement.h"
#include "../UI/pathsettingsdialog.h"

class VisPath : public VisElement
{
public:
    VisPath();
    ~VisPath();

    virtual QString toString(void);

    virtual void render(QWidget* widget, QPainter* painter, RobotData *robot, bool selected, QRectF rect);

    virtual QDialog* getSettingsDialog(void);

    void setSelectedOnly(bool enable);
    bool getSelectedOnly(void);

private:
    PathSettingsDialog* settingsDialog;

    bool selectedOnly;
};

#endif // VISPATH_H
