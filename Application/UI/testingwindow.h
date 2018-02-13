#ifndef TESTINGWINDOW_H
#define TESTINGWINDOW_H

#include <QString>
#include <QDialog>
#include <QTextEdit>
#include <QListWidget>

#include "../DataModel/datamodel.h"

class TestingWindow;
typedef bool (*FPTR)(TestingWindow* window);

typedef struct {
    QString name;
    FPTR function;
} Test;

class TestingWindow : public QDialog
{
    Q_OBJECT
public:
    TestingWindow();

    void initUI(void);

    QTextEdit* console;

    DataModel* dataModel;

private slots:
    void runButtonClicked(void);
    void runAllButtonClicked(void);

private:
    void accept(void);
    void reject(void);

    bool runTest(Test test);

    QListWidget* testList;

    Test testArray[9];
};

#endif // TESTINGWINDOW_H
