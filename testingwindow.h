#ifndef TESTINGWINDOW_H
#define TESTINGWINDOW_H

#include <QDialog>
#include <QTextEdit>
#include <QListWidget>

class TestingWindow : public QDialog
{
    Q_OBJECT
public:
    TestingWindow();

    void initUI(void);

private slots:
    void runButtonClicked(void);
    void runAllButtonClicked(void);

private:
    void accept(void);
    void reject(void);

    QTextEdit* console;
    QListWidget* testList;
};

#endif // TESTINGWINDOW_H
