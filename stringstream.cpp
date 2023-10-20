#include <iostream>
#include <sstream>
#include <string>

int main() {
    std::string input;

    // Ask the user to input a string of integers separated by commas
    std::getline(std::cin, input);

    // Create a string stream to split the input by commas
    std::istringstream ss(input);
    std::string token;

    // Iterate through the tokens (integers) and print each one on a new line
    while (std::getline(ss, token, ',')) {
        int number;
        std::istringstream(token) >> number;
        std::cout << number << std::endl;
    }

    return 0;
}
