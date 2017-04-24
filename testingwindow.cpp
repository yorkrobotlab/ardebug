#include "testingwindow.h"

#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

QString assertTrue(bool condition, bool* pass) {
    if (condition) {
        return QString("TRUE");
    } else {
        *pass = false;
        return QString("FALSE");
    }
}

bool robotInsertionTestFunction(TestingWindow* window) {
    bool pass = true;

    window->console->append("Create data model.");
    window->dataModel = new DataModel();

    window->console->append("Insert Watchdog Packet [0 0 Robot0].");
    window->dataModel->newData("0 0 Robot0");
    window->console->append("ASSERT: Robot Count = 1: " + assertTrue(window->dataModel->getRobotCount() == 1, &pass));

    window->console->append("Insert State Packet [1 1 STATE].");
    window->dataModel->newData("1 1 STATE");
    window->console->append("ASSERT: Robot Count = 2: " + assertTrue(window->dataModel->getRobotCount() == 2, &pass));

    window->console->append("Insert Position Packet [2 2 0.3 0.3 45].");
    window->dataModel->newData("2 2 0.3 0.3 45");
    window->console->append("ASSERT: Robot Count = 3: " + assertTrue(window->dataModel->getRobotCount() == 3, &pass));

    window->console->append("Insert IR Packet [3 3 100 200 300 400 1000 2000 3000 4000].");
    window->dataModel->newData("3 3 100 200 300 400 1000 2000 3000 4000");
    window->console->append("ASSERT: Robot Count = 4: " + assertTrue(window->dataModel->getRobotCount() == 4, &pass));

    window->console->append("Insert Background IR Packet [4 4 100 200 300 400 1000 2000 3000 4000].");
    window->dataModel->newData("4 4 100 200 300 400 1000 2000 3000 4000");
    window->console->append("ASSERT: Robot Count = 5: " + assertTrue(window->dataModel->getRobotCount() == 5, &pass));

    window->console->append("Insert Custom Data Packet [6 6 Key Value].");
    window->dataModel->newData("6 6 Key Value");
    window->console->append("ASSERT: Robot Count = 6: " + assertTrue(window->dataModel->getRobotCount() == 6, &pass));

    window->console->append("Delete data model.");
    delete window->dataModel;

    return pass;
}

bool nameDataTestFunction(TestingWindow* window) {
    bool pass = true;

    window->console->append("Create data model.");
    window->dataModel = new DataModel();

    window->console->append("Insert Watchdog Packet [0 0 Name_Zero].");
    window->console->append("Insert Watchdog Packet [1 0 Name_One].");
    window->console->append("Insert Watchdog Packet [2 0 Name_Two].");
    window->dataModel->newData("0 0 Name_Zero");
    window->dataModel->newData("1 0 Name_One");
    window->dataModel->newData("2 0 Name_Two");
    window->console->append("ASSERT: Robot 0 Name = Name_Zero: " + assertTrue(window->dataModel->getRobotByID(0)->getName() == "Name_Zero", &pass));
    window->console->append("ASSERT: Robot 1 Name = Name_One: " + assertTrue(window->dataModel->getRobotByID(1)->getName() == "Name_One", &pass));
    window->console->append("ASSERT: Robot 2 Name = Name_Two: " + assertTrue(window->dataModel->getRobotByID(2)->getName() == "Name_Two", &pass));

    window->console->append("Insert Watchdog Packet [0 0 New_Name_Zero].");
    window->console->append("Insert Watchdog Packet [1 0 New_Name_One].");
    window->console->append("Insert Watchdog Packet [2 0 New_Name_Two].");
    window->dataModel->newData("0 0 New_Name_Zero");
    window->dataModel->newData("1 0 New_Name_One");
    window->dataModel->newData("2 0 New_Name_Two");
    window->console->append("ASSERT: Robot 0 Name = New_Name_Zero: " + assertTrue(window->dataModel->getRobotByID(0)->getName() == "New_Name_Zero", &pass));
    window->console->append("ASSERT: Robot 1 Name = New_Name_One: " + assertTrue(window->dataModel->getRobotByID(1)->getName() == "New_Name_One", &pass));
    window->console->append("ASSERT: Robot 2 Name = New_Name_Two: " + assertTrue(window->dataModel->getRobotByID(2)->getName() == "New_Name_Two", &pass));

    window->console->append("Delete data model.");
    delete window->dataModel;

    return pass;
}

bool stateDataTestFunction(TestingWindow* window) {
    bool pass = true;

    window->console->append("Create data model.");
    window->dataModel = new DataModel();

    window->console->append("Delete data model.");
    delete window->dataModel;

    return pass;
}

TestingWindow::TestingWindow() {
    Test robotInsertionTest;
    robotInsertionTest.name = "Robot Insertion Test";
    robotInsertionTest.function = &robotInsertionTestFunction;

    Test nameDataTest;
    nameDataTest.name = "Name Data Test";
    nameDataTest.function = &nameDataTestFunction;

    Test stateDataTest;
    stateDataTest.name = "State Data Test";
    stateDataTest.function = &stateDataTestFunction;

    testArray[0] = robotInsertionTest;
    testArray[1] = nameDataTest;
    testArray[2] = stateDataTest;

    initUI();
}

void TestingWindow::initUI(void) {
    this->setModal(true);
    this->setGeometry(200, 200, 1200, 675);
    this->setWindowTitle("Unit Tests");

    testList = new QListWidget();
    console = new QTextEdit();

    console->setReadOnly(true);

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

    for (int i = 0; i < 3; i++) {
        testList->addItem(testArray[i].name);
    }
}

void TestingWindow::accept(void) {
    QDialog::accept();
}

void TestingWindow::reject(void) {
    QDialog::reject();
}

void TestingWindow::runButtonClicked(void) {
    console->clear();

    QModelIndexList indexes = testList->selectionModel()->selectedIndexes();

    if (indexes.count() > 0) {
        runTest(testArray[indexes.at(0).row()]);
    }
}

void TestingWindow::runAllButtonClicked(void) {
    console->clear();

    for (int i = 0; i < 3; i++) {
        runTest(testArray[i]);
    }
}

bool TestingWindow::runTest(Test test) {
    console->append("+++ " + test.name + " +++");
    console->append("START");

    bool pass = test.function(this);

    console->append("END");

    if (pass) {
        console->append("Result: PASS");
    } else {
        console->append("Result: FAIL");
    }

    console->append("+++++++++++++++++++");

    return pass;
}
