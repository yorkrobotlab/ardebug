#ifndef VISPOSITION_H
#define VISPOSITION_H

#include "viselement.h"

class VisPosition : public VisElement
{
public:
    VisPosition(void);

    virtual QString toString(void);

    virtual void render(QWidget* widget, QPainter* painter, RobotData *robot, bool selected, QRectF rect);

    virtual QDialog* getSettingsDialog(void);
};

#endif // VISPOSITION_H
