#ifndef VISELEMENT_H
#define VISELEMENT_H

#include <QString>
#include <QDialog>

#include "../DataModel/robotdata.h"

class VisElement
{
private:
    bool enabled;

public:
    virtual ~VisElement(void) { }

    void setEnabled(bool en) { enabled = en; }
    bool isEnabled(void) { return enabled; }

    virtual void render(QWidget* widget, QPainter* painter, RobotData* robot, bool selected, QRectF rect) = 0;
};

Q_DECLARE_METATYPE(VisElement*)

#endif // VISELEMENT_H
