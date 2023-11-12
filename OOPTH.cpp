#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class Tank {
public:
    double capacity;
    double fuel_quantity;
    int tankNumber;
    bool connected;
    bool isBroken;
    Tank() : capacity(0.0), fuel_quantity(0.0), tankNumber(0),connected(false),isBroken(false){}
};

class Engine {
public:
    double fuel_per_second;
    bool status;
    bool valveopen;
    vector<Tank> tanks;

    Engine() : fuel_per_second(0.0), status(false),valveopen(false) {}
    void openValve() {
        valveopen = true;
        cout << "Valve opened\n";
    }
    void closeValve() {
        valveopen = false;
        cout << "Valve closed\n";
    }

    void start() {
        if (valveopen) {

            bool atLeastOneWorkingTankConnected = false;
            for (const Tank& tank : tanks) {
                if (tank.connected && !tank.isBroken) {
                    atLeastOneWorkingTankConnected = true;
                    break;  // No need to check further, at least one working tank is connected
                }
            }

            if (atLeastOneWorkingTankConnected) {
                status = true;
                cout << "Engine started\n";
            }
            else {
                status = false;
                cout << "Engine cannot start. No working tanks connected or all connected tanks are broken.\n";
            }
        }
        else {
            status = false;
            cout << "Engine cannot start. Valve is not open.\n";
        }
    }

    void stop() {
        if (status) {
            status = false;
            cout << "Engine stopped\n";
        }
        else {
            cout<<"Engine is not running.\n";
        }

    }

    void update() {
        if (status) {
            bool atLeastOneWorkingTankConnected=false;
            for ( Tank& tank : tanks) {
                tank.fuel_quantity -= fuel_per_second;
                cout << "Tank " << tank.tankNumber << " Fuel quantity: " << tank.fuel_quantity << "\n";
                if (tank.connected && !tank.isBroken) {
                    atLeastOneWorkingTankConnected = true;
                    break;  // No need to check further, at least one working tank is connected
                }
            }
            if (atLeastOneWorkingTankConnected) {
                cout << "Engine running\n";
            }
            else {
                cout << "Engine not running\n";
            }

        }
        else {
            
        }
    }

    bool addTank(double capacity, int tankNumber) {
        if (tanks.size() < MAX_TANKS) {
            Tank newTank;
            newTank.capacity = capacity;
            newTank.tankNumber = tankNumber;
            tanks.push_back(newTank);
            cout << "Tank " << tankNumber << " added with capacity: " << capacity << "\n";
            return true;
        }
        else {
            cout << "Cannot add more tanks. Maximum limit reached.\n";
            return false;
        }
    }

    bool removeTank(int tankNumber) {
        auto it = find_if(tanks.begin(), tanks.end(), [tankNumber](const Tank& tank) {
            return tank.tankNumber == tankNumber;
            });

        if (it != tanks.end()) {
            tanks.erase(it);
            cout << "Tank " << tankNumber << " removed\n";
            return true;
        }
        else {
            cout << "Tank " << tankNumber << " not found. Cannot remove.\n";
            return false;
        }
    }


    void connectTank(int tankNumber) {
        if (valveopen) {
            for (Tank& tank : tanks) {
                if (tank.tankNumber == tankNumber) {
                    tank.connected = true;
                    cout << "Tank " << tankNumber << " connected to the engine\n";
                    return;
                }
            }
            cout << "Tank " << tankNumber << " not found. Cannot connect.\n";
        }
        else
            cout << "valve is not open.\n";

    }

    void disconnectTank(int tankNumber) {
        auto it = find_if(tanks.begin(), tanks.end(), [tankNumber](const Tank& tank) {
            return tank.tankNumber == tankNumber;
            });

        if (it != tanks.end()) {
            it->connected = false; 
            cout << "Tank " << tankNumber << " disconnected from the engine\n";
        }
        else {
            cout << "Tank " << tankNumber << " not found. Cannot disconnect.\n";
        }
        bool atLeastOneWorkingTankConnected = false;
        for (const Tank& tank : tanks) {
            if (tank.connected && !tank.isBroken) {
                atLeastOneWorkingTankConnected = true;
                break;  
            }
            else
                status = false;
        }
    }
    void listTanks() const {
        if (tanks.empty()) {
            cout << "No tanks available.\n";
        }
        else {
            cout << "List of tanks:\n";
            for (const Tank& tank : tanks) {
                cout << "Tank " << tank.tankNumber << " - Capacity: " << tank.capacity << " - "
                    << (tank.connected ? "Connected" : "Not connected") << "\n";
            }
        }
    }
    void breakTank(int tankNumber) {
        auto it = find_if(tanks.begin(), tanks.end(), [tankNumber](const Tank& tank) {
            return tank.tankNumber == tankNumber;
            });

        if (it != tanks.end()) {
            if (it->isBroken) {
                cout << "Tank " << tankNumber << " is already broken.\n";
            }
            else {
                it->isBroken = true;
                cout << "Tank " << tankNumber << " is now broken.\n";
            }
        }
        else {
            cout << "Tank " << tankNumber << " not found.\n";
        }
    }

    void fixTank(int tankNumber) {
        auto it = find_if(tanks.begin(), tanks.end(), [tankNumber](const Tank& tank) {
            return tank.tankNumber == tankNumber;
            });

        if (it != tanks.end()) {
            if (!it->isBroken) {
                cout << "Tank " << tankNumber << " is not broken.\n";
            }
            else {
                it->isBroken = false;
                cout << "Tank " << tankNumber << " is now fixed.\n";
            }
        }
        else {
            cout << "Tank " << tankNumber << " not found. \n";
        }
    }

private:
    static const size_t MAX_TANKS = 5;
};

class Processor {
public:
    static void processCommands(Engine& engine, const string& inputFileName, const string& outputFileName) {
        ifstream inputFile(inputFileName);
        ofstream outputFile(outputFileName);

        if (!inputFile.is_open() || !outputFile.is_open()) {
            cerr << "Error opening files\n";
            return;
        }
        streambuf* coutbuf = cout.rdbuf();
        cout.rdbuf(outputFile.rdbuf());

        string command;
        double capacity;
        int tankNumber;

        while (inputFile >> command) {
            if (command == "start_engine") {
                engine.start();
            }
            else if (command == "stop_engine") {
                engine.stop();
            }
            else if (command == "UPDATE") {
                engine.update();
            }
            else if (command == "add_fuel_tank") {
                if (inputFile >> capacity >> tankNumber) {
                    engine.addTank(capacity, tankNumber);
                }
                else {
                    cerr << "Invalid ADDTANK command. Missing capacity or tank number.\n";
                }
            }
            else if (command=="remove_fuel_tank") {
                if (inputFile >> tankNumber) {
                    engine.removeTank(tankNumber);
                }
                else {
                    cerr << "Invalid REMOVETANK command. Missing tank number.\n";
                }
            }
            else if (command == "connect_fuel_tank_to_engine") {
                if (inputFile >> tankNumber) {
                    engine.connectTank(tankNumber);
                }
                else {
                    cerr << "Invalid CONNECTTANK command. Missing tank number.\n";
                }
            }
            else if (command == "disconnect_fuel_tank_from_engine") {
                if (inputFile >> tankNumber) {
                    engine.disconnectTank(tankNumber);
                }
                else {
                    cerr << "Invalid DISCONNECTTANK command. Missing tank number.\n";
                }
             
            }
            else if (command=="list_fuel_tanks") {
                engine.listTanks();
            }
            else if (command == "break_fuel_tank") {
                if (inputFile >> tankNumber) {
                    engine.breakTank(tankNumber);
                }
                else {
                    cerr << "BREAKTANK command. Missing tank number.\n";
                }
            }
            else if (command == "repair_fuel_tank") {
                if (inputFile >> tankNumber) {
                    engine.fixTank(tankNumber);
                }
            }
            else if (command == "open_valve") {
                engine.openValve();
            }
            else if (command == "close_valve") {
                engine.closeValve();
            }
            
            else {
                cerr << "Unknown command: " << command << "\n";
            }
        }

        inputFile.close();
        outputFile.close();
    }
};

int main() {
    Engine myEngine;
    Processor::processCommands(myEngine, "input.txt", "output.txt");

    return 0;
}
