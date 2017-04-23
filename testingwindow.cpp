#include "testingwindow.h"

#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

TestingWindow::TestingWindow() {
    initUI();
}

void TestingWindow::initUI(void) {
    this->setModal(true);
    this->setGeometry(200, 200, 1200, 675);
    this->setWindowTitle("Unit Tests");

    testList = new QListWidget();
    console = new QTextEdit();

    QPushButton* runButton = new QPushButton("Run");
    QPushButton* runAllButton = new QPushButton("Run All");

    QObject::connect(runButton, SIGNAL(clicked(bool)), this, SLOT(runButtonClicked()));
    QObject::connect(runAllButton, SIGNAL(clicked(bool)), this, SLOT(runAllButtonClicked()));

    QHBoxLayout* runButtonBox = new QHBoxLayout();
    runButtonBox->addWidget(runButton);
    runButtonBox->addWidget(runAllButton);

    QVBoxLayout* testListBox = new QVBoxLayout();
    testListBox->addWidget(new QLabel("Tests:"));
    testListBox->addWidget(testList);
    testListBox->addLayout(runButtonBox);

    QVBoxLayout* consoleBox = new QVBoxLayout();
    consoleBox->addWidget(new QLabel("Output:"));
    consoleBox->addWidget(console);

    QHBoxLayout* mainBox = new QHBoxLayout();
    mainBox->addLayout(testListBox, 1);
    mainBox->addLayout(consoleBox, 3);

    this->setLayout(mainBox);

    // Temp tests
    //QListWidgetItem* item = new QListWidgetItem;
    //item->setData(Qt::DisplayRole, "Test 1");
    testList->addItem("Test 1");
    testList->addItem("Test 2");

    for (int i = 0; i < 100; i++) {
        console->append("New Line");
    }
}

void TestingWindow::accept(void) {
    QDialog::accept();
}

void TestingWindow::reject(void) {
    QDialog::reject();
}

void TestingWindow::runButtonClicked(void) {
    QDialog::reject();
}

void TestingWindow::runAllButtonClicked(void) {
    QDialog::reject();
}
