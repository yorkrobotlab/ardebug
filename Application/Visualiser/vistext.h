#ifndef VISID_H
#define VISID_H

#include "viselement.h"
#include "../UI/idsettingsdialog.h"

class VisText : public VisElement
{
public:
    VisText();
    ~VisText();

    virtual void render(QWidget* widget, QPainter* painter, RobotData *robot, bool selected, QRectF rect);

    virtual QDialog* getSettingsDialog(void);

    void setText(QString newText);
    QString getText();

    void addLine(QString newLine);
    void resetText();

private:
    IDSettingsDialog* settingsDialog;

    QString text;
};

#endif // VISID_H
