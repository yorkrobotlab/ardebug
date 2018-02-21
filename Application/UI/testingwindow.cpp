/* testingwindow.cpp
 *
 * This class encapsulates a window for running and displaying the outputs of
 * the data model unit tests.
 *
 * (C) Alistair Jewers Jan 2017
 */

#include "testingwindow.h"
#include "../Core/settings.h"

#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

/* assertTrue
 * Utility function for asserting a truth
 */
QString assertTrue(bool condition, bool* pass) {
    if (condition) {
        return QString("TRUE");
    } else {
        *pass = false;
        return QString("FALSE");
    }
}

/* robotInsertionTest
 * Tests that robots are correctly inserted into the data model when detected
 */
bool robotInsertionTestFunction(TestingWindow* window) {
    bool pass = true;

    window->console->append("Create data model.");
    window->dataModel = new DataModel();

    window->console->append("Insert Watchdog Packet [0 0 Robot0].");
    window->dataModel->newData("{\"id\":\"testRobotId\"}");
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

    window->console->append("Insert Watchdog Packet [6 0 Robot6].");
    window->dataModel->newData("0 0 Robot0");
    window->console->append("ASSERT: Robot Count = 6: " + assertTrue(window->dataModel->getRobotCount() == 6, &pass));

    window->console->append("Insert State Packet [5 1 STATE2].");
    window->dataModel->newData("1 1 STATE2");
    window->console->append("ASSERT: Robot Count = 6: " + assertTrue(window->dataModel->getRobotCount() == 6, &pass));

    window->console->append("Insert Position Packet [4 2 0.5 0.5 90].");
    window->dataModel->newData("2 2 0.5 0.5 90");
    window->console->append("ASSERT: Robot Count = 6: " + assertTrue(window->dataModel->getRobotCount() == 6, &pass));

    window->console->append("Insert IR Packet [3 3 101 201 301 401 1001 2001 3001 4001].");
    window->dataModel->newData("3 3 101 201 301 401 1001 2001 3001 4001");
    window->console->append("ASSERT: Robot Count = 6: " + assertTrue(window->dataModel->getRobotCount() == 6, &pass));

    window->console->append("Insert Background IR Packet [2 4 101 201 301 401 1001 2001 3001 4001].");
    window->dataModel->newData("2 4 101 201 301 401 1001 2001 3001 4001");
    window->console->append("ASSERT: Robot Count = 6: " + assertTrue(window->dataModel->getRobotCount() == 6, &pass));

    window->console->append("Insert Custom Data Packet [0 6 Key Value].");
    window->dataModel->newData("0 6 Key Value");
    window->console->append("ASSERT: Robot Count = 6: " + assertTrue(window->dataModel->getRobotCount() == 6, &pass));

    window->console->append("Delete data model.");
    delete window->dataModel;

    return pass;
}

/* nameDataTestFunction
 * Tests that name data is inserted into the model correctly.
 */
bool nameDataTestFunction(TestingWindow* window) {
    bool pass = true;

    window->console->append("Create data model.");
    window->dataModel = new DataModel();

    window->console->append("Insert Watchdog Packet [0 0 Name_Zero].");
    window->console->append("Insert Watchdog Packet [1 0 Name_One].");
    window->console->append("Insert Watchdog Packet [2 0 Name_Two].");
    window->dataModel->newData("{\"id\":\"testRobotId\"}");
    window->dataModel->newData("0 0 Name_Zero");
    window->dataModel->newData("1 0 Name_One");
    window->dataModel->newData("2 0 Name_Two");
    window->console->append("ASSERT: Robot 0 Name = Name_Zero: " + assertTrue(window->dataModel->getRobotByIndex(0)->getID() == "Name_Zero", &pass));
    window->console->append("ASSERT: Robot 1 Name = Name_One: " + assertTrue(window->dataModel->getRobotByIndex(1)->getID() == "Name_One", &pass));
    window->console->append("ASSERT: Robot 2 Name = Name_Two: " + assertTrue(window->dataModel->getRobotByIndex(2)->getID() == "Name_Two", &pass));

    window->console->append("Insert Watchdog Packet [0 0 New_Name_Zero].");
    window->console->append("Insert Watchdog Packet [1 0 New_Name_One].");
    window->console->append("Insert Watchdog Packet [2 0 New_Name_Two].");
    window->dataModel->newData("0 0 New_Name_Zero");
    window->dataModel->newData("1 0 New_Name_One");
    window->dataModel->newData("2 0 New_Name_Two");
    window->console->append("ASSERT: Robot 0 Name = New_Name_Zero: " + assertTrue(window->dataModel->getRobotByIndex(0)->getID() == "New_Name_Zero", &pass));
    window->console->append("ASSERT: Robot 1 Name = New_Name_One: " + assertTrue(window->dataModel->getRobotByIndex(1)->getID() == "New_Name_One", &pass));
    window->console->append("ASSERT: Robot 2 Name = New_Name_Two: " + assertTrue(window->dataModel->getRobotByIndex(2)->getID() == "New_Name_Two", &pass));

    window->console->append("Delete data model.");
    delete window->dataModel;

    return pass;
}

/* stateDataTestFunction
 * Tests that state data is inserted into the model correctly.
 */
bool stateDataTestFunction(TestingWindow* window) {
    bool pass = true;

    window->console->append("Create data model.");
    window->dataModel = new DataModel();

    window->console->append("Insert State Packet [0 1 STATE_1].");
    window->console->append("Insert State Packet [1 1 STATE_2].");
    window->console->append("Insert State Packet [2 1 STATE_3].");
    window->dataModel->newData("{\"id\":\"testRobotId\"}");
    window->dataModel->newData("0 1 STATE_1");
    window->dataModel->newData("1 1 STATE_2");
    window->dataModel->newData("2 1 STATE_3");
    window->console->append("ASSERT: Robot 0 State = STATE_1: " + assertTrue(window->dataModel->getRobotByIndex(0)->getState() == "STATE_1", &pass));
    window->console->append("ASSERT: Robot 1 State = STATE_2: " + assertTrue(window->dataModel->getRobotByIndex(1)->getState() == "STATE_2", &pass));
    window->console->append("ASSERT: Robot 2 State = STATE_3: " + assertTrue(window->dataModel->getRobotByIndex(2)->getState() == "STATE_3", &pass));

    window->console->append("Insert State Packet [0 1 STATE_4].");
    window->console->append("Insert State Packet [1 1 STATE_5].");
    window->console->append("Insert State Packet [2 1 STATE_6].");
    window->dataModel->newData("0 1 STATE_4");
    window->dataModel->newData("1 1 STATE_5");
    window->dataModel->newData("2 1 STATE_6");
    window->console->append("ASSERT: Robot 0 State = STATE_4: " + assertTrue(window->dataModel->getRobotByIndex(0)->getState() == "STATE_4", &pass));
    window->console->append("ASSERT: Robot 1 State = STATE_5: " + assertTrue(window->dataModel->getRobotByIndex(1)->getState() == "STATE_5", &pass));
    window->console->append("ASSERT: Robot 2 State = STATE_6: " + assertTrue(window->dataModel->getRobotByIndex(2)->getState() == "STATE_6", &pass));

    window->console->append("Delete data model.");
    delete window->dataModel;

    return pass;
}

/* positionDataTestFunction
 * Tests that position data is correctly inserted into the data model.
 */
bool positionDataTestFunction(TestingWindow* window) {
    bool pass = true;

    window->console->append("Create data model.");
    window->dataModel = new DataModel();

    window->console->append("Insert Position Packet [0 2 0.1 0.2 45].");
    window->console->append("Insert Position Packet [1 2 0.3 0.4 60].");
    window->console->append("Insert Position Packet [2 2 0.7 0.8 110].");
    window->dataModel->newData("{\"id\":\"testRobotId\"}");
    window->dataModel->newData("0 2 0.1 0.2 45");
    window->dataModel->newData("1 2 0.3 0.4 60");
    window->dataModel->newData("2 2 0.7 0.8 110");
    window->console->append("ASSERT: Robot 0 position.x = 0.1: " + assertTrue(fabs(window->dataModel->getRobotByIndex(0)->getPos().position.x - 0.1f) < 0.0000001, &pass));
    window->console->append("ASSERT: Robot 0 position.y = 0.2: " + assertTrue(fabs(window->dataModel->getRobotByIndex(0)->getPos().position.y - 0.2f) < 0.0000001, &pass));
    window->console->append("ASSERT: Robot 0 angle = 45: " + assertTrue(window->dataModel->getRobotByIndex(0)->getAngle() == 45, &pass));
    window->console->append("ASSERT: Robot 1 position.x = 0.3: " + assertTrue(fabs(window->dataModel->getRobotByIndex(1)->getPos().position.x - 0.3f) < 0.0000001, &pass));
    window->console->append("ASSERT: Robot 1 position.y = 0.4: " + assertTrue(fabs(window->dataModel->getRobotByIndex(1)->getPos().position.y - 0.4f) < 0.0000001, &pass));
    window->console->append("ASSERT: Robot 1 angle = 60: " + assertTrue(window->dataModel->getRobotByIndex(1)->getAngle() == 60, &pass));
    window->console->append("ASSERT: Robot 2 position.x = 0.7: " + assertTrue(fabs(window->dataModel->getRobotByIndex(2)->getPos().position.x - 0.7f) < 0.0000001, &pass));
    window->console->append("ASSERT: Robot 2 position.y = 0.8: " + assertTrue(fabs(window->dataModel->getRobotByIndex(2)->getPos().position.y - 0.8f) < 0.0000001, &pass));
    window->console->append("ASSERT: Robot 2 angle = 110: " + assertTrue(window->dataModel->getRobotByIndex(2)->getAngle() == 110, &pass));

    window->console->append("Insert Position Packet [0 2 0.9 0.8 315].");
    window->console->append("Insert Position Packet [1 2 0.7 0.6 300].");
    window->console->append("Insert Position Packet [2 2 0.3 0.2 250].");
    window->dataModel->newData("0 2 0.9 0.8 315");
    window->dataModel->newData("1 2 0.7 0.6 300");
    window->dataModel->newData("2 2 0.3 0.2 250");
    window->console->append("ASSERT: Robot 0 position.x = 0.9: " + assertTrue(fabs(window->dataModel->getRobotByIndex(0)->getPos().position.x - 0.9f) < 0.0000001, &pass));
    window->console->append("ASSERT: Robot 0 position.y = 0.8: " + assertTrue(fabs(window->dataModel->getRobotByIndex(0)->getPos().position.y - 0.8f) < 0.0000001, &pass));
    window->console->append("ASSERT: Robot 0 angle = 315: " + assertTrue(window->dataModel->getRobotByIndex(0)->getAngle() == 315, &pass));
    window->console->append("ASSERT: Robot 1 position.x = 0.7: " + assertTrue(fabs(window->dataModel->getRobotByIndex(1)->getPos().position.x - 0.7f) < 0.0000001, &pass));
    window->console->append("ASSERT: Robot 1 position.y = 0.6: " + assertTrue(fabs(window->dataModel->getRobotByIndex(1)->getPos().position.y - 0.6f) < 0.0000001, &pass));
    window->console->append("ASSERT: Robot 1 angle = 300: " + assertTrue(window->dataModel->getRobotByIndex(1)->getAngle() == 300, &pass));
    window->console->append("ASSERT: Robot 2 position.x = 0.3: " + assertTrue(fabs(window->dataModel->getRobotByIndex(2)->getPos().position.x - 0.3f) < 0.0000001, &pass));
    window->console->append("ASSERT: Robot 2 position.y = 0.2: " + assertTrue(fabs(window->dataModel->getRobotByIndex(2)->getPos().position.y - 0.2f) < 0.0000001, &pass));
    window->console->append("ASSERT: Robot 2 angle = 250: " + assertTrue(window->dataModel->getRobotByIndex(2)->getAngle() == 250, &pass));

    window->console->append("Delete data model.");
    delete window->dataModel;

    return pass;
}

/* irDataTestFunction
 * Tests that IR data is correctly inserted into the data model
 */
bool irDataTestFunction(TestingWindow* window) {
    bool pass = true;

    window->console->append("Create data model.");
    window->dataModel = new DataModel();

    window->console->append("Insert IR Data Packet [0 3 101 102 103 104 105 106 107 108].");
    window->dataModel->newData("{\"id\":\"testRobotId\"}");
    window->dataModel->newData("0 3 101 102 103 104 105 106 107 108");
    window->console->append("ASSERT: Robot 0 IR data 1 = 101: " + assertTrue(window->dataModel->getRobotByID(0)->getProximitySensorData(0) == 101, &pass));
    window->console->append("ASSERT: Robot 0 IR data 2 = 102: " + assertTrue(window->dataModel->getRobotByID(0)->getProximitySensorData(1) == 102, &pass));
    window->console->append("ASSERT: Robot 0 IR data 3 = 103: " + assertTrue(window->dataModel->getRobotByID(0)->getProximitySensorData(2) == 103, &pass));
    window->console->append("ASSERT: Robot 0 IR data 4 = 104: " + assertTrue(window->dataModel->getRobotByID(0)->getProximitySensorData(3) == 104, &pass));
    window->console->append("ASSERT: Robot 0 IR data 5 = 105: " + assertTrue(window->dataModel->getRobotByID(0)->getProximitySensorData(4) == 105, &pass));
    window->console->append("ASSERT: Robot 0 IR data 6 = 106: " + assertTrue(window->dataModel->getRobotByID(0)->getProximitySensorData(5) == 106, &pass));
    window->console->append("ASSERT: Robot 0 IR data 7 = 107: " + assertTrue(window->dataModel->getRobotByID(0)->getProximitySensorData(6) == 107, &pass));
    window->console->append("ASSERT: Robot 0 IR data 8 = 108: " + assertTrue(window->dataModel->getRobotByID(0)->getProximitySensorData(7) == 108, &pass));

    window->console->append("Insert IR Data Packet [0 3 0 500 1000 1500 2000 2500 3000 4095].");
    window->dataModel->newData("0 3 0 500 1000 1500 2000 2500 3000 4095");
    window->console->append("ASSERT: Robot 0 IR data 1 = 0: " + assertTrue(window->dataModel->getRobotByID(0)->getProximitySensorData(0) == 0, &pass));
    window->console->append("ASSERT: Robot 0 IR data 2 = 500: " + assertTrue(window->dataModel->getRobotByID(0)->getProximitySensorData(1) == 500, &pass));
    window->console->append("ASSERT: Robot 0 IR data 3 = 1000: " + assertTrue(window->dataModel->getRobotByID(0)->getProximitySensorData(2) == 1000, &pass));
    window->console->append("ASSERT: Robot 0 IR data 4 = 1500: " + assertTrue(window->dataModel->getRobotByID(0)->getProximitySensorData(3) == 1500, &pass));
    window->console->append("ASSERT: Robot 0 IR data 5 = 2000: " + assertTrue(window->dataModel->getRobotByID(0)->getProximitySensorData(4) == 2000, &pass));
    window->console->append("ASSERT: Robot 0 IR data 6 = 2500: " + assertTrue(window->dataModel->getRobotByID(0)->getProximitySensorData(5) == 2500, &pass));
    window->console->append("ASSERT: Robot 0 IR data 7 = 3000: " + assertTrue(window->dataModel->getRobotByID(0)->getProximitySensorData(6) == 3000, &pass));
    window->console->append("ASSERT: Robot 0 IR data 8 = 4095: " + assertTrue(window->dataModel->getRobotByID(0)->getProximitySensorData(7) == 4095, &pass));

    window->console->append("Delete data model.");
    delete window->dataModel;

    return pass;
}

/* customDataTestFunction
 * Tests that custom data is correctly inserted into the data model
 */
bool customDataTestFunction(TestingWindow* window) {
    bool pass = true;

    window->console->append("Create data model.");
    window->dataModel = new DataModel();

    window->console->append("Insert Custom Data Packet [0 6 Key1 Value1].");
    window->console->append("Insert Custom Data Packet [1 6 Key1 Value2].");
    window->console->append("Insert Custom Data Packet [2 6 Key1 Value3].");
    window->dataModel->newData("{\"id\":\"testRobotId\"}");
    window->dataModel->newData("0 6 Key1 Value1");
    window->dataModel->newData("1 6 Key1 Value2");
    window->dataModel->newData("2 6 Key1 Value3");
    window->console->append("ASSERT: Robot 0 Custom data for key Key1 = Value1: " + assertTrue(window->dataModel->getRobotByIndex(0)->getCustomData("Key1") == "Value1", &pass));
    window->console->append("ASSERT: Robot 1 Custom data for key Key1 = Value2: " + assertTrue(window->dataModel->getRobotByIndex(1)->getCustomData("Key1") == "Value2", &pass));
    window->console->append("ASSERT: Robot 2 Custom data for key Key1 = Value3: " + assertTrue(window->dataModel->getRobotByIndex(2)->getCustomData("Key1") == "Value3", &pass));

    window->console->append("Insert Custom Data Packet [0 6 Key2 Value4].");
    window->console->append("Insert Custom Data Packet [1 6 Key2 Value5].");
    window->console->append("Insert Custom Data Packet [2 6 Key2 Value6].");
    window->dataModel->newData("0 6 Key2 Value4");
    window->dataModel->newData("1 6 Key2 Value5");
    window->dataModel->newData("2 6 Key2 Value6");
    window->console->append("ASSERT: Robot 0 Custom data for key Key2 = Value4: " + assertTrue(window->dataModel->getRobotByIndex(0)->getCustomData("Key2") == "Value4", &pass));
    window->console->append("ASSERT: Robot 1 Custom data for key Key2 = Value5: " + assertTrue(window->dataModel->getRobotByIndex(1)->getCustomData("Key2") == "Value5", &pass));
    window->console->append("ASSERT: Robot 2 Custom data for key Key2 = Value6: " + assertTrue(window->dataModel->getRobotByIndex(2)->getCustomData("Key2") == "Value6", &pass));

    window->console->append("Insert Custom Data Packet [0 6 Key1 Value7].");
    window->console->append("Insert Custom Data Packet [1 6 Key1 Value8].");
    window->console->append("Insert Custom Data Packet [2 6 Key1 Value9].");
    window->dataModel->newData("0 6 Key1 Value7");
    window->dataModel->newData("1 6 Key1 Value8");
    window->dataModel->newData("2 6 Key1 Value9");
    window->console->append("ASSERT: Robot 0 Custom data for key Key1 = Value7: " + assertTrue(window->dataModel->getRobotByIndex(0)->getCustomData("Key1") == "Value7", &pass));
    window->console->append("ASSERT: Robot 1 Custom data for key Key1 = Value8: " + assertTrue(window->dataModel->getRobotByIndex(1)->getCustomData("Key1") == "Value8", &pass));
    window->console->append("ASSERT: Robot 2 Custom data for key Key1 = Value9: " + assertTrue(window->dataModel->getRobotByIndex(2)->getCustomData("Key1") == "Value9", &pass));

    window->console->append("ASSERT: Robot 0 Custom data for key Key2 = Value4: " + assertTrue(window->dataModel->getRobotByIndex(0)->getCustomData("Key2") == "Value4", &pass));
    window->console->append("ASSERT: Robot 1 Custom data for key Key2 = Value5: " + assertTrue(window->dataModel->getRobotByIndex(1)->getCustomData("Key2") == "Value5", &pass));
    window->console->append("ASSERT: Robot 2 Custom data for key Key2 = Value6: " + assertTrue(window->dataModel->getRobotByIndex(2)->getCustomData("Key2") == "Value6", &pass));

    window->console->append("Delete data model.");
    delete window->dataModel;

    return pass;
}

/* positionHistoryTestFunction
 * Tests that the position history is correctly created from position data
 */
bool positionHistoryTestFunction(TestingWindow* window) {
    bool pass = true;

    window->console->append("Save previous sampling interval.");
    int interval = Settings::instance()->getPosHistorySampleInterval();

    window->console->append("Set sampling interval to 2.");
    Settings::instance()->setPosHistorySampleInterval(2);

    window->console->append("Create data model.");
    window->dataModel = new DataModel();

    window->console->append("Insert Position Packet [0 2 0.00 0.00 90] (Sampled).");
    window->console->append("Insert Position Packet [0 2 0.05 0.05 90].");
    window->console->append("Insert Position Packet [0 2 0.10 0.10 90] (Sampled).");
    window->console->append("Insert Position Packet [0 2 0.15 0.15 90].");
    window->console->append("Insert Position Packet [0 2 0.20 0.20 90] (Sampled).");
    window->console->append("Insert Position Packet [0 2 0.25 0.25 90].");
    window->console->append("Insert Position Packet [0 2 0.30 0.30 90] (Sampled).");
    window->console->append("Insert Position Packet [0 2 0.35 0.35 90].");
    window->console->append("Insert Position Packet [0 2 0.40 0.40 90] (Sampled).");
    window->console->append("Insert Position Packet [0 2 0.45 0.45 90].");
    window->console->append("Insert Position Packet [0 2 0.50 0.50 90] (Sampled).");
    window->console->append("Insert Position Packet [0 2 0.55 0.55 90].");
    window->console->append("Insert Position Packet [0 2 0.60 0.60 90] (Sampled).");
    window->console->append("Insert Position Packet [0 2 0.65 0.65 90].");
    window->console->append("Insert Position Packet [0 2 0.70 0.70 90] (Sampled).");
    window->console->append("Insert Position Packet [0 2 0.75 0.75 90].");
    window->console->append("Insert Position Packet [0 2 0.80 0.80 90] (Sampled).");
    window->console->append("Insert Position Packet [0 2 0.85 0.85 90].");
    window->console->append("Insert Position Packet [0 2 0.90 0.90 90] (Sampled).");
    window->console->append("Insert Position Packet [0 2 0.95 0.95 90].");
    window->dataModel->newData("{\"id\":\"testRobotId\"}");
    window->dataModel->newData("0 2 0.00 0.00 90");
    window->dataModel->newData("0 2 0.05 0.05 90");
    window->dataModel->newData("0 2 0.10 0.10 90");
    window->dataModel->newData("0 2 0.15 0.15 90");
    window->dataModel->newData("0 2 0.20 0.20 90");
    window->dataModel->newData("0 2 0.25 0.25 90");
    window->dataModel->newData("0 2 0.30 0.30 90");
    window->dataModel->newData("0 2 0.35 0.35 90");
    window->dataModel->newData("0 2 0.40 0.40 90");
    window->dataModel->newData("0 2 0.45 0.45 90");
    window->dataModel->newData("0 2 0.50 0.50 90");
    window->dataModel->newData("0 2 0.55 0.55 90");
    window->dataModel->newData("0 2 0.60 0.60 90");
    window->dataModel->newData("0 2 0.65 0.65 90");
    window->dataModel->newData("0 2 0.70 0.70 90");
    window->dataModel->newData("0 2 0.75 0.75 90");
    window->dataModel->newData("0 2 0.80 0.80 90");
    window->dataModel->newData("0 2 0.85 0.85 90");
    window->dataModel->newData("0 2 0.90 0.90 90");
    window->dataModel->newData("0 2 0.95 0.95 90");

    Pose posHistory[POS_HISTORY_COUNT];
    window->dataModel->getRobotByID(0)->getPosHistory(posHistory);
    window->console->append("ASSERT: Robot 0 x-position at history sample N = 0.90: " + assertTrue(fabs(posHistory[0].position.x - 0.90f) < 0.0000001, &pass));
    window->console->append("ASSERT: Robot 0 y-position at history sample N = 0.90: " + assertTrue(fabs(posHistory[0].position.y - 0.90f) < 0.0000001, &pass));
    window->console->append("ASSERT: Robot 0 x-position at history sample N-1 = 0.80: " + assertTrue(fabs(posHistory[1].position.x - 0.80f) < 0.0000001, &pass));
    window->console->append("ASSERT: Robot 0 y-position at history sample N-1 = 0.80: " + assertTrue(fabs(posHistory[1].position.y - 0.80f) < 0.0000001, &pass));
    window->console->append("ASSERT: Robot 0 x-position at history sample N-2 = 0.70: " + assertTrue(fabs(posHistory[2].position.x - 0.70f) < 0.0000001, &pass));
    window->console->append("ASSERT: Robot 0 y-position at history sample N-2 = 0.70: " + assertTrue(fabs(posHistory[2].position.y - 0.70f) < 0.0000001, &pass));
    window->console->append("ASSERT: Robot 0 x-position at history sample N-3 = 0.60: " + assertTrue(fabs(posHistory[3].position.x - 0.60f) < 0.0000001, &pass));
    window->console->append("ASSERT: Robot 0 y-position at history sample N-3 = 0.60: " + assertTrue(fabs(posHistory[3].position.y - 0.60f) < 0.0000001, &pass));
    window->console->append("ASSERT: Robot 0 x-position at history sample N-4 = 0.50: " + assertTrue(fabs(posHistory[4].position.x - 0.50f) < 0.0000001, &pass));
    window->console->append("ASSERT: Robot 0 y-position at history sample N-4 = 0.50: " + assertTrue(fabs(posHistory[4].position.y - 0.50f) < 0.0000001, &pass));
    window->console->append("ASSERT: Robot 0 x-position at history sample N-5 = 0.40: " + assertTrue(fabs(posHistory[5].position.x - 0.40f) < 0.0000001, &pass));
    window->console->append("ASSERT: Robot 0 y-position at history sample N-5 = 0.40: " + assertTrue(fabs(posHistory[5].position.y - 0.40f) < 0.0000001, &pass));
    window->console->append("ASSERT: Robot 0 x-position at history sample N-6 = 0.40: " + assertTrue(fabs(posHistory[6].position.x - 0.30f) < 0.0000001, &pass));
    window->console->append("ASSERT: Robot 0 y-position at history sample N-6 = 0.40: " + assertTrue(fabs(posHistory[6].position.y - 0.30f) < 0.0000001, &pass));
    window->console->append("ASSERT: Robot 0 x-position at history sample N-7 = 0.40: " + assertTrue(fabs(posHistory[7].position.x - 0.20f) < 0.0000001, &pass));
    window->console->append("ASSERT: Robot 0 y-position at history sample N-7 = 0.40: " + assertTrue(fabs(posHistory[7].position.y - 0.20f) < 0.0000001, &pass));
    window->console->append("ASSERT: Robot 0 x-position at history sample N-8 = 0.40: " + assertTrue(fabs(posHistory[8].position.x - 0.10f) < 0.0000001, &pass));
    window->console->append("ASSERT: Robot 0 y-position at history sample N-8 = 0.40: " + assertTrue(fabs(posHistory[8].position.y - 0.10f) < 0.0000001, &pass));
    window->console->append("ASSERT: Robot 0 x-position at history sample N-9 = 0.40: " + assertTrue(fabs(posHistory[9].position.x - 0.00f) < 0.0000001, &pass));
    window->console->append("ASSERT: Robot 0 y-position at history sample N-9 = 0.40: " + assertTrue(fabs(posHistory[9].position.y - 0.00f) < 0.0000001, &pass));

    window->console->append("Delete data model.");
    delete window->dataModel;

    window->console->append("Restore previous sampling interval.");
    Settings::instance()->setPosHistorySampleInterval(interval);

    return pass;
}

/* stateTransitionHistoryTestFunction
 * Tests that the state transition history is correctly created from state data
 */
bool stateTransitionHistoryTestFunction(TestingWindow* window) {
    bool pass = true;

    window->console->append("Create data model.");
    window->dataModel = new DataModel();

    window->console->append("Insert State Packet [0 1 STATE_1].");
    window->console->append("Insert State Packet [0 1 STATE_2].");
    window->console->append("Insert State Packet [0 1 STATE_3].");
    window->console->append("Insert State Packet [0 1 STATE_4].");
    window->dataModel->newData("{\"id\":\"testRobotId\"}");
    window->dataModel->newData("0 1 STATE_1");
    window->dataModel->newData("0 1 STATE_2");
    window->dataModel->newData("0 1 STATE_3");
    window->dataModel->newData("0 1 STATE_4");

    QStringList stateTransitionStringList = window->dataModel->getRobotByID(0)->getStateTransitionList()->stringList();
    window->console->append("ASSERT: State transition N = STATE_3 -> STATE_4: " + assertTrue(stateTransitionStringList.at(0).endsWith("STATE_3 -> STATE_4"), &pass));
    window->console->append("ASSERT: State transition N-1 = STATE_2 -> STATE_3: " + assertTrue(stateTransitionStringList.at(1).endsWith("STATE_2 -> STATE_3"), &pass));
    window->console->append("ASSERT: State transition N-2 = STATE_1 -> STATE_2: " + assertTrue(stateTransitionStringList.at(2).endsWith("STATE_1 -> STATE_2"), &pass));
    window->console->append("ASSERT: State transition N-3 = Unknown -> STATE_1: " + assertTrue(stateTransitionStringList.at(3).endsWith("Unknown -> STATE_1"), &pass));

    window->console->append("Delete data model.");
    delete window->dataModel;

    return pass;
}

/* badDataTestFunction
 * Tests that badly formed data is ignored
 */
bool badDataTestFunction(TestingWindow* window) {
    bool pass = true;

    window->console->append("Create data model.");
    window->dataModel = new DataModel();

    window->console->append("Insert Watchdog Packet [0 0 Robot0] (Well Formed).");
    window->dataModel->newData("0 0 Robot0");

    window->console->append("Insert State Packet [1 1 STATE] (Well Formed).");
    window->dataModel->newData("1 1 STATE");

    window->console->append("ASSERT: Robot Count = 1: " + assertTrue(window->dataModel->getRobotCount() == 2, &pass));

    window->console->append("Insert Bad Packet [A 0 RobotA] (Incorrectly Formed).");
    window->console->append("Insert Bad Packet [1 RobotA] (Incorrectly Formed).");
    window->console->append("Insert Bad Packet [A ] (Incorrectly Formed).");
    window->console->append("Insert Bad Packet [1 7 DATA] (No such type).");
    window->console->append("Insert Bad Packet [-1 0 NegBot] (Negative robot ID).");
    window->dataModel->newData("A 0 RobotA");
    window->dataModel->newData("1 RobotA");
    window->dataModel->newData("A ");
    window->dataModel->newData("1 7 DATA");
    window->dataModel->newData("-1 0 NegBot");
    window->console->append("ASSERT: Robot Count = 1: " + assertTrue(window->dataModel->getRobotCount() == 2, &pass));
    window->console->append(QString::number(window->dataModel->getRobotCount()));

    window->console->append("Delete data model.");
    delete window->dataModel;

    return pass;
}

/* Constructor
 * Initialises the tests
 */
TestingWindow::TestingWindow() {
    // Initialise all the tests
    Test robotInsertionTest;
    robotInsertionTest.name = "Robot Insertion Test";
    robotInsertionTest.function = &robotInsertionTestFunction;

    Test nameDataTest;
    nameDataTest.name = "Name Data Test";
    nameDataTest.function = &nameDataTestFunction;

    Test stateDataTest;
    stateDataTest.name = "State Data Test";
    stateDataTest.function = &stateDataTestFunction;

    Test positionDataTest;
    positionDataTest.name = "Position Data Test";
    positionDataTest.function = &positionDataTestFunction;

    Test irDataTest;
    irDataTest.name = "IR Data Test";
    irDataTest.function = &irDataTestFunction;

    Test customDataTest;
    customDataTest.name = "Custom Data Test";
    customDataTest.function = &customDataTestFunction;

    Test positionHistoryTest;
    positionHistoryTest.name = "Position History Test";
    positionHistoryTest.function = &positionHistoryTestFunction;

    Test stateTransitionHistoryTest;
    stateTransitionHistoryTest.name = "State Transition History Test";
    stateTransitionHistoryTest.function = &stateTransitionHistoryTestFunction;

    Test badDataTest;
    badDataTest.name = "Bad Data Test";
    badDataTest.function = &badDataTestFunction;

    // Create the test array
    testArray[0] = robotInsertionTest;
    testArray[1] = nameDataTest;
    testArray[2] = stateDataTest;
    testArray[3] = positionDataTest;
    testArray[4] = irDataTest;
    testArray[5] = customDataTest;
    testArray[6] = positionHistoryTest;
    testArray[7] = stateTransitionHistoryTest;
    testArray[8] = badDataTest;

    // Initialise the GUI
    initUI();
}

/* initUI
 * Initialises the UI
 */
void TestingWindow::initUI(void) {
    // Set up window
    this->setModal(true);
    this->setGeometry(200, 200, 1200, 675);
    this->setWindowTitle("Unit Tests");

    // Create the window components
    testList = new QListWidget();
    console = new QTextEdit();

    console->setReadOnly(true);

    QPushButton* runButton = new QPushButton("Run");
    QPushButton* runAllButton = new QPushButton("Run All");

    QObject::connect(runButton, SIGNAL(clicked(bool)), this, SLOT(runButtonClicked()));
    QObject::connect(runAllButton, SIGNAL(clicked(bool)), this, SLOT(runAllButtonClicked()));

    // Lay out the run buttons in a row
    QHBoxLayout* runButtonBox = new QHBoxLayout();
    runButtonBox->addWidget(runButton);
    runButtonBox->addWidget(runAllButton);

    // Lay out the run buttons below the test list
    QVBoxLayout* testListBox = new QVBoxLayout();
    testListBox->addWidget(new QLabel("Tests:"));
    testListBox->addWidget(testList);
    testListBox->addLayout(runButtonBox);

    // Lay out the output console with a label above
    QVBoxLayout* consoleBox = new QVBoxLayout();
    consoleBox->addWidget(new QLabel("Output:"));
    consoleBox->addWidget(console);

    // Combine the layouts horzontally
    QHBoxLayout* mainBox = new QHBoxLayout();
    mainBox->addLayout(testListBox, 1);
    mainBox->addLayout(consoleBox, 3);

    // Set the main layout
    this->setLayout(mainBox);

    // Populate the test list
    for (int i = 0; i < 9; i++) {
        testList->addItem(testArray[i].name);
    }
}

/* accept
 * Overrides QDialog accept function. Called to close the window.
 */
void TestingWindow::accept(void) {
    QDialog::accept();
}

/* reject
 * Overrides QDialog reject function. Called to close the window.
 */
void TestingWindow::reject(void) {
    QDialog::reject();
}

/* runButtonClicked
 * Runs the selected test
 */
void TestingWindow::runButtonClicked(void) {
    console->clear();

    QModelIndexList indexes = testList->selectionModel()->selectedIndexes();

    if (indexes.count() > 0) {
        runTest(testArray[indexes.at(0).row()]);
    }
}

/* runAllButtonClicked
 * Runs all of the tests
 */
void TestingWindow::runAllButtonClicked(void) {
    console->clear();

    for (int i = 0; i < 9; i++) {
        runTest(testArray[i]);
    }
}

/* runTest
 * Run a specific test
 */
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
