//added fill_tank
//added print_tank_info
//added print_fuel_tank_count;
//added give_back_fuel <quantity>; 
//added list_fuel_tanks
//added list_connected_tanks

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

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
            Tank newTank(capacity, 0.0, tankNumber, false, false);
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
        }
        else {
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
                cout << "Tank " << tank.tankNumber
                    << " - Capacity: " << tank.capacity
                    << " - Tank fuel quantity " << tank.fuel_quantity
                    << " - " << (tank.connected ? "Connected" : "Not connected")
                    << " - " << (tank.isBroken ? "Broken" : "Not broken") << "\n";
            }
        }
    }

    static void breakTank(vector<Tank>& tanks, int tankNumber) {
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

    static void fixTank(vector<Tank>& tanks, int tankNumber) {
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
        if (quantity > capacity) {
            cout << "Tank can not take that much fuel. \n";
        }
        else {
            fuel_quantity += quantity;
            cout << "Fill with the quantity of " << quantity << endl;
        }
    }

    static void printTankInfo(const vector<Tank>& tanks, int tankNumber) {
        auto it = find_if(tanks.begin(), tanks.end(), [tankNumber](const Tank& tank) {
            return tank.tankNumber == tankNumber;
            });

        if (it != tanks.end()) {
            cout << "Tank " << tankNumber
                << " - Capacity: " << it->capacity
                << " - Tank fuel quantity " << it->fuel_quantity
                << " - " << (it->connected ? "Connected" : "Not connected")
                << " - " << (it->isBroken ? "Broken" : "Not broken") << "\n";
        }
        else {
            cout << "Tank " << tankNumber << " not found.\n";
        }
    }
};

class Engine {
private:
    static Engine* instance;

    Engine() : fuel_per_second(5.5), status(false), valveopen(false), internalTankcapcacity(55.0), internalTankFuelQuantity(55.0) {}

    const double fuel_per_second;
    const double internalTankcapcacity;
    double internalTankFuelQuantity;
    bool status;
    bool valveopen;
    vector<Tank> tanks;

public:
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
    }

    void addTank(double capacity, int tankNumber) {
        Tank::addTank(tanks, capacity, tankNumber);
    }

    void removeTank(int tankNumber) {
        Tank::removeTank(tanks, tankNumber);
    }

    void connectTank(int tankNumber) {
        if (!valveopen) {
            cout << "Valve is not open. Cannot connect tank.\n";
            return;
        }

        auto tankIt = find_if(tanks.begin(), tanks.end(), [tankNumber](const Tank& tank) {
            return tank.tankNumber == tankNumber;
            });

        if (tankIt != tanks.end()) {
            if (!tankIt->connected) {

                tankIt->connected = true;
                cout << "Tank " << tankNumber << " connected to the engine\n";
            }
            else {
                cout << "Tank " << tankNumber << " is already connected.\n";
            }
        }
        else {
            cout << "Tank " << tankNumber << " not found. Cannot connect.\n";
        }
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
            else {
                status = false;
            }
        }
    }
    
    void giveBackFuel(double quantity) {
        internalTankFuelQuantity += quantity;
        cout << "Given back " << quantity << " fuel\n";
    }

    void listTanks() const {
        Tank::listTanks(tanks);
    }

    void breakTank(int tankNumber) {
        Tank::breakTank(tanks, tankNumber);
    }

    void fixTank(int tankNumber) {
        Tank::fixTank(tanks, tankNumber);
    }

    void wait(int seconds) {
        double requiredFuel = seconds * fuel_per_second;
        cout << "Required Fuel: " << requiredFuel << endl;
        double temp = internalTankFuelQuantity;

        if (internalTankFuelQuantity >= requiredFuel) {
            cout << "Consumed from internal tank" << endl;
            internalTankFuelQuantity -= requiredFuel;
            requiredFuel = 0;
        }

        if (internalTankFuelQuantity < 20 || requiredFuel > internalTankFuelQuantity) {
            double gapInternalTank = internalTankcapcacity - internalTankFuelQuantity;
            cout << "Taking fuel from other tanks" << endl;
            for (Tank& tank : tanks) {
                if (tank.connected && !tank.isBroken && tank.fuel_quantity != 0) {
                    if (tank.fuel_quantity >= gapInternalTank) {
                        tank.fuel_quantity -= gapInternalTank;
                        internalTankFuelQuantity += gapInternalTank;
                        gapInternalTank = 0;
                    }
                    else if (tank.fuel_quantity < gapInternalTank) {
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
                    cout << "There is not enough fuel" << endl;
                }
            }
        }

        if (internalTankFuelQuantity >= requiredFuel) {
            cout << "Consumed from internal tank" << endl;
            internalTankFuelQuantity -= requiredFuel
                ;
        }
    }

    void fillTank(int tankNumber, double quantity) {
        if (!valveopen) {
            cout << "Valve is not open. Cannot fill tank.\n";
            return;
        }

        auto it = find_if(tanks.begin(), tanks.end(), [tankNumber](const Tank& tank) {
            return tank.tankNumber == tankNumber;
            });

        if (it != tanks.end()) {
            it->fillTank(quantity);
        }
        else {
            cout << "Tank " << tankNumber << " not found. Cannot fill tank.\n";
        }
    }

    double getFuelQuantity() const {
        return internalTankFuelQuantity;
    }

    double getFuelPerSecond() const {
        return fuel_per_second;
    }

    const vector<Tank>& getTanks() const {
        return tanks;
    }
 
    void printTotalFuelQuantity() const {
        double totalFuel = internalTankFuelQuantity;
        for (const Tank& tank : tanks) {
            totalFuel += tank.fuel_quantity;
        }

        cout << "Total Fuel Quantity: " << totalFuel << endl;
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
        double seconds, quantity;

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
            else if (command == "wait") {
                if (inputFile >> seconds) {
                    engine.wait(static_cast<int>(seconds));
                }
            }
            else if (command == "get_fuel_quantity") {
                double fuel = engine.getFuelQuantity();
                cout << fuel << endl;
            }
            else if (command == "fill_tank") {
                if (inputFile >> tankNumber >> quantity) {
                    auto currentFuelQuantity = engine.getFuelQuantity();
                    auto requiredFuel = quantity - currentFuelQuantity;
                    if (requiredFuel > 0) {
                        engine.wait(static_cast<int>(requiredFuel / engine.getFuelPerSecond()));
                    }
                    auto remainingQuantity = quantity - engine.getFuelQuantity();
                    engine.fillTank(tankNumber, remainingQuantity);
                }
                else {
                    cerr << "FILLTANK command. Missing tank number or quantity.\n";
                }
            }
            else if (command == "print_tank_info") {
                if (inputFile >> tankNumber) {
                    Tank::printTankInfo(engine.getTanks(), tankNumber);
                }
                else {
                    cerr << "PRINTTANKINFO command. Missing tank number.\n";
                }
            }
            else if (command == "give_back_fuel") {
                if (inputFile >> quantity) {
                    engine.giveBackFuel(quantity);
                }
                else {
                    cerr << "GIVEBACKFUEL command. Missing quantity.\n";
                }
            }
            else if (command == "print_total_fuel_quantity") {
                engine.printTotalFuelQuantity();
            }
            else if (command == "print_fuel_tank_count") {
                int tankCount = engine.getTanks().size();
                cout << "Fuel tank count: " << tankCount << endl;
            }
            else if (command == "list_fuel_tanks") {
                engine.listTanks();
            }
            else if (command == "list_connected_tanks") {
                cout << "Connected tanks:\n";
                const auto& tanks = engine.getTanks();
                bool foundConnectedTank = false;

                for (const auto& tank : tanks) {
                    if (tank.connected) {
                        foundConnectedTank = true;
                        cout << "Tank " << tank.tankNumber << "\n";
                    }
                }

                if (!foundConnectedTank) {
                    cout << "No tanks are currently connected.\n";
                }
            }

            else if (command == "stop_simulation") {
                break;
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
