#ifndef VISELEMENT_H
#define VISELEMENT_H

#include <QString>
#include <QDialog>

#include <opencv2/opencv.hpp>

#include "robotdata.h"

enum VisType { ID, NAME, STATE, POSITION, DIRECTION, PROXIMITY, PATH };

class VisElement
{
private:
    VisType type;
    bool enabled;

public:
    virtual ~VisElement(void) { }

    void setType(VisType t) { type = t; }
    VisType getType(void) { return type; }

    void setEnabled(bool en) { enabled = en; }
    bool isEnabled(void) { return enabled; }

    virtual QString toString(void) = 0;

    virtual void render(cv::Mat image, RobotData* robot, bool selected) = 0;

    virtual QDialog* getSettingsDialog(void) = 0;
};

Q_DECLARE_METATYPE(VisElement*)

#endif // VISELEMENT_H
