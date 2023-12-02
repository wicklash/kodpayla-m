#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;
static const size_t MAX_TANKS = 5;
class Tank {
public:
    double capacity;
    double fuel_quantity;
    int tankNumber;
    bool connected;
    bool isBroken;
    Tank() : capacity(0.0), fuel_quantity(0.0), tankNumber(0), connected(false), isBroken(false) {}

    Tank(double capacity, double fuel_quantity, int tankNumber, bool connected, bool isBroken)
        : capacity(capacity), fuel_quantity(fuel_quantity), tankNumber(tankNumber), connected(connected), isBroken(isBroken) {}

    static bool addTank(vector<Tank>& tanks, double capacity, int tankNumber) {
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

    static bool removeTank(vector<Tank>& tanks, int tankNumber) {
        auto it = find_if(tanks.begin(), tanks.end(), [tankNumber](const Tank& tank) {
            return tank.tankNumber == tankNumber;
        });

        if (it != tanks.end()) {
            tanks.erase(it);
            cout << "Tank " << tankNumber << " removed\n";
            return true;
        } else {
            cout << "Tank " << tankNumber << " not found. Cannot remove.\n";
            return false;
        }
    }

    static void listTanks(const vector<Tank>& tanks) {
        if (tanks.empty()) {
            cout << "No tanks available.\n";
        }
        else {
            cout << "List of tanks:\n";
            for (const Tank& tank : tanks) {
                cout<< "Tank " << tank.tankNumber 
                    << " - Capacity: " << tank.capacity 
                    << "-Tank fuel quantity "<< tank.fuel_quantity 
                    << " - "<< (tank.connected ? "Connected" : "Not connected") << "\n";
            }
        }
    }

    static void breakTank(vector<Tank>& tanks,int tankNumber) {
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

    static void fixTank(vector<Tank>& tanks,int tankNumber) {
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

    void fillTank(double quantity) {
        if (quantity>capacity) {
            cout << "Tank can not take that much fuel. \n";
        }
        else {
        fuel_quantity += quantity;
        cout << "fill with the quantity of " << quantity << endl;
        }

    }

};

class Engine {
private:
    static Engine* instance;
    Engine() : fuel_per_second(5.5), status(false), valveopen(false) {}
    
    const double internalTankcapcacity = 55;
    double internalTankFuelQuantity = 55;

public:
    const double fuel_per_second;
    bool status;
    bool valveopen;
    vector<Tank> tanks;

    static Engine* getInstance() {
        if (instance == nullptr) {
            instance = new Engine();
        }
        return instance;
    }

    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;
    
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
                    break;
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
            cout << "Engine is not running.\n";
        }

    }

    void update() {
        if (status) {
            bool atLeastOneWorkingTankConnected = false;
            for (Tank& tank : tanks) {
                tank.fuel_quantity -= fuel_per_second;
                cout << "Tank " << tank.tankNumber << " Fuel quantity: " << tank.fuel_quantity << "\n";
                if (tank.connected && !tank.isBroken) {
                    atLeastOneWorkingTankConnected = true;
                    break;
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

    void AddTank(double capacity, int tankNumber) {
        Tank::addTank(tanks, capacity, tankNumber);
    }
    void removeTank(int tankNumber) {
        Tank::removeTank(tanks, tankNumber);
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
        Tank::listTanks(tanks);
    }
    void breakTank(int tankNumber){
        Tank::breakTank(tanks,tankNumber);
    }

    void fixTank(int tankNumber) {
        Tank::fixTank(tanks, tankNumber);
    }

    void wait(int seconds) {
        double requiredFuel = seconds * fuel_per_second;
        cout << "requredFuel: " << requiredFuel << endl;
        double temp = internalTankFuelQuantity;

        if (internalTankFuelQuantity >= requiredFuel) {
            cout << "consumed from internal tank" << endl;
            internalTankFuelQuantity -= requiredFuel;
            requiredFuel = 0;
        }

        if (internalTankFuelQuantity < 20||requiredFuel>internalTankFuelQuantity) {
            double gapinternalTank = internalTankcapcacity - internalTankFuelQuantity;
            cout << "taking fuel from other tanks" << endl;
            for (Tank& tank : tanks) {
                if (tank.connected && !tank.isBroken && tank.fuel_quantity!=0) {
                    if (tank.fuel_quantity>=gapinternalTank) {
                        tank.fuel_quantity -= gapinternalTank;
                        internalTankFuelQuantity += gapinternalTank;
                        gapinternalTank = 0;
                    }
                    else if (tank.fuel_quantity < gapinternalTank) {
                        internalTankFuelQuantity += tank.fuel_quantity;
                        tank.fuel_quantity = 0;
                    }
                    temp = internalTankFuelQuantity;
                    internalTankFuelQuantity -= requiredFuel;
                    requiredFuel -= temp;

                    if (internalTankFuelQuantity > 20) {
                        break;
                    }
                 }
                else {
                    cout << "there is not enough fuel" << endl;
                }
            }

        }
        if (internalTankFuelQuantity >= requiredFuel) {
            cout << "consumed from internal tank" << endl;
            internalTankFuelQuantity -= requiredFuel;
            requiredFuel = 0;
        }
    }


    double getsFuelQuantity() const {
        return internalTankFuelQuantity;
    }

    double getsfuelquantity() {
        return internalTankFuelQuantity;
    }

    ~Engine() {
        delete instance;
    }

};

Engine* Engine::instance = nullptr;

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
        double seconds,quantity;

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
                    engine.AddTank(capacity, tankNumber);
                }
                else {
                    cerr << "Invalid ADDTANK command. Missing capacity or tank number.\n";
                }
            }
            else if (command == "remove_fuel_tank") {
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
            else if (command == "list_fuel_tanks") {
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
            else if (command=="wait") {   
                if (inputFile >> seconds) {
                    engine.wait(seconds);
                }    
            }
            else if (command=="get_fuel_quantity") {
                double fuel = engine.getsfuelquantity();
                cout << fuel << endl;
            }
            else if (command=="fill_tank") {
                if (inputFile >> tankNumber >> quantity) {
                    engine.tanks[tankNumber-1].fillTank(quantity);
                }
                
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
    Engine* myEngine = Engine::getInstance();
    Processor::processCommands(*myEngine, "input.txt", "output.txt");

    return 0;
}
