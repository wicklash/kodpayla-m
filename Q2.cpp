#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class City {
public:
    string name;
    int deliveryTime;
    City* left;
    City* right;

    City(string cityName, int time) {
        name = cityName;
        deliveryTime = time;
        left = right = nullptr;
    }
};

void insert(City*& root, string cityName, int time) {
    if (root == nullptr) {
        root = new City(cityName, time);
    }
    else {
        if (time < root->deliveryTime) {
            insert(root->left, cityName, time);
        }
        else {
            insert(root->right, cityName, time);
        }
    }
}

void inOrderTraversal(City* root, vector<string>& deliveryOrder) {
    if (root) {
        inOrderTraversal(root->left, deliveryOrder);
        deliveryOrder.push_back(root->name);
        inOrderTraversal(root->right, deliveryOrder);
    }
}

int main() {
    vector<pair<string, int>> cities = {
        {"Bucharest", 5},     
        {"Fagaras", 7},
        {"Neamt", 9},  
        {"Vaslui", 8}
    };

    City* root = nullptr;

    for (const auto& city : cities) {
        insert(root, city.first, city.second);
    }

    vector<string> deliveryOrder;
    inOrderTraversal(root, deliveryOrder);

    cout << "Delivery Order: ";
    for (const auto& city : deliveryOrder) {
        cout << city << " -> ";
    }
    cout << endl;

    return 0;
}
