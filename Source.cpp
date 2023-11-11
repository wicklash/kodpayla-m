#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>

using namespace std;

class FuelTank;

class InputFileReader {
public:

    vector<string> readCommands(const string& filename) {
        vector<string> commands;
        ifstream inputFile(filename);

        if (inputFile.is_open()) {
            string line;
            while (getline(inputFile, line)) {
                commands.push_back(line);
            }
            inputFile.close();
        }
        else {
            cerr << "Error: Unable to open input file." << endl;
        }

        return commands;
    }
};

class OutputFileWriter {
public:
    void writeCommands(const string& filename, const vector<string>& commands) {
        ofstream outputFile(filename);

        if (outputFile.is_open()) {
            for (const auto& command : commands) {
                outputFile << command << endl;
            }
            outputFile.close();
            cout << "Commands written to " << filename << endl;
        }
        else {
            cerr << "Error: Unable to open output file." << endl;
        }
    }
};

//empty class

class FuelTank {
private:

    double capacity;
    double fuel_quantity;
    bool isBroken;
    bool valveOpen;

public:
    
};

//empty class

class Engine {
private:

    double fuel_per_second;
    bool status;
    FuelTank* internalTank;

public:

};

int main() {

    InputFileReader fileReader;
    OutputFileWriter fileWriter;

    vector<string> commands = fileReader.readCommands("input.txt");
    vector<string> executedCommands; // To store executed commands

    for (const auto& command : commands) {

        executedCommands.push_back(command); // Store executed command
    }
    
    fileWriter.writeCommands("output.txt", executedCommands);

    return 0;
}
