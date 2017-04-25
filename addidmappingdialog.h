#ifndef ADDIDMAPPINGDIALOG_H
#define ADDIDMAPPINGDIALOG_H

#include <QObject>
#include <QDialog>
#include <QCheckBox>
#include <QLineEdit>

class AddIDMAppingDialog : public QDialog
{
public:
    AddIDMAppingDialog(void);

private:
    void accept(void);
    void reject(void);

    QLineEdit* arucoIDEdit;
    QLineEdit* robotIDEdit;
};

#endif // ADDIDMAPPINGDIALOG_H
