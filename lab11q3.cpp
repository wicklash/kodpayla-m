#include <iostream>
#include <vector>
#include <algorithm> // for std::search

class City {
public:
    std::string name;
    int deliveryTime;
    City* left;
    City* right;

    City(std::string cityName, int time)
        : name(cityName), deliveryTime(time), left(nullptr), right(nullptr) {}
};

class CityDeliveryOrder {
public:
    CityDeliveryOrder() : root(nullptr) {}

    ~CityDeliveryOrder() {
        clear(root);
    }

    void insert(const std::string& cityName, int time) {
        insertHelper(root, cityName, time);
    }

    void inOrderTraversal(std::vector<std::string>& deliveryOrder) const {
        inOrderTraversalHelper(root, deliveryOrder);
    }

    bool isSubtree(const CityDeliveryOrder& subtree) const {
        return isSubtreeHelper(root, subtree.root);
    }

private:
    void insertHelper(City*& node, const std::string& cityName, int time) {
        if (!node) {
            node = new City(cityName, time);
        }
        else if (time < node->deliveryTime) {
            insertHelper(node->left, cityName, time);
        }
        else {
            insertHelper(node->right, cityName, time);
        }
    }

    void inOrderTraversalHelper(const City* node, std::vector<std::string>& deliveryOrder) const {
        if (node) {
            inOrderTraversalHelper(node->left, deliveryOrder);
            deliveryOrder.push_back(node->name);
            inOrderTraversalHelper(node->right, deliveryOrder);
        }
    }

    void clear(City*& node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            delete node;
            node = nullptr;
        }
    }

    bool isSubtreeHelper(const City* mainTree, const City* subtreeRoot) const {
        std::vector<std::string> mainTreeOrder, subtreeOrder;

        // Get the in-order traversals for both trees
        inOrderTraversalHelper(mainTree, mainTreeOrder);
        inOrderTraversalHelper(subtreeRoot, subtreeOrder);

        // Check if the subtreeOrder is a subsequence of mainTreeOrder
        return std::search(mainTreeOrder.begin(), mainTreeOrder.end(), subtreeOrder.begin(), subtreeOrder.end()) != mainTreeOrder.end();
    }

    City* root;
};

int main() {
    CityDeliveryOrder cityOrder;

    // Insert nodes to create the main tree
    cityOrder.insert("Bucharest", 5);
    cityOrder.insert("Pitesti", 3);
    cityOrder.insert("Fagaras", 7);
    cityOrder.insert("Rimnicu-Vilcea", 2);
    cityOrder.insert("Sibiu", 4);
    cityOrder.insert("Urziceni", 6);
    cityOrder.insert("Vaslui", 8);

    // Display the main tree
    std::vector<std::string> deliveryOrder;
    cityOrder.inOrderTraversal(deliveryOrder);

    

    // Test with different subtrees
    CityDeliveryOrder subtree1;
    subtree1.insert("Bucharest", 5);
    subtree1.insert("Pitesti", 3);
    subtree1.insert("Sibiu", 4);
    if (cityOrder.isSubtree(subtree1)) {
        std::cout << "Subtree 1 is a subtree of the main tree." << std::endl;
    }
    else {
        std::cout << "Subtree 1 is not a subtree of the main tree." << std::endl;
    }

    CityDeliveryOrder subtree2;
    subtree2.insert("Fagaras", 7);
    subtree2.insert("Sibiu", 4);

    if (cityOrder.isSubtree(subtree2)) {
        std::cout << "Subtree 2 is a subtree of the main tree." << std::endl;
    }
    else {
        std::cout << "Subtree 2 is not a subtree of the main tree." << std::endl;
    }

    return 0;
}


