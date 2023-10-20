#include <iostream>
#include <string>

int main() {
    std::string a, b;

    // Read two strings separated by a new line
    std::getline(std::cin, a);
    std::getline(std::cin, b);

    // Calculate the lengths of strings a and b
    int len_a = a.length();
    int len_b = b.length();

    // Concatenate strings a and b
    std::string concatenated = a + b;

    // Swap the first characters of a and b to create a' and b'
    std::string a_swapped = b[0] + a.substr(1);
    std::string b_swapped = a[0] + b.substr(1);

    // Output the results as specified
    std::cout << len_a << " " << len_b << std::endl;
    std::cout << concatenated << std::endl;
    std::cout << a_swapped << " " << b_swapped << std::endl;

    return 0;
}
