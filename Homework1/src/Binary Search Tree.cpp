#include <iostream>
#include <random>
#include <cmath>

using namespace std;

class BST {
private:
    struct Node {
        int key;
        Node* left;
        Node* right;

        Node(int k) : key(k), left(nullptr), right(nullptr) {}
    };

    Node* root;

    // ========================
    // (a) Insert Function
    // ========================
    Node* insert(Node* node, int key) {
        if (node == nullptr) return new Node(key);

        if (key < node->key)
            node->left = insert(node->left, key);
        else if (key > node->key)
            node->right = insert(node->right, key);

        return node;
    }

    // ========================
    // Height Calculation
    // ========================
    int height(Node* node) const {
        if (node == nullptr) return 0;

        int leftHeight = height(node->left);
        int rightHeight = height(node->right);

        return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
    }

    // ========================
    // (b) Find Minimum
    // ========================
    Node* findMin(Node* node) {
        while (node != nullptr && node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    // ========================
    // (b) Remove Function
    // ========================
    Node* remove(Node* node, int key) {
        if (node == nullptr) return nullptr;

        if (key < node->key) {
            node->left = remove(node->left, key);
        }
        else if (key > node->key) {
            node->right = remove(node->right, key);
        }
        else {
            // case 1: no child
            if (node->left == nullptr && node->right == nullptr) {
                delete node;
                return nullptr;
            }
            // case 2: one child
            else if (node->left == nullptr) {
                Node* temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == nullptr) {
                Node* temp = node->left;
                delete node;
                return temp;
            }
            // case 3: two children
            else {
                Node* temp = findMin(node->right);
                node->key = temp->key;
                node->right = remove(node->right, temp->key);
            }
        }

        return node;
    }

    void clear(Node* node) {
        if (node == nullptr) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }

public:
    BST() : root(nullptr) {}

    ~BST() {
        clear(root);
    }

    void insert(int key) {
        root = insert(root, key);
    }

    void remove(int key) {
        root = remove(root, key);
    }

    int height() const {
        return height(root);
    }
};

int main() {
    // ========================
    // (a) Random Insertion Test
    // ========================
    int testValues[] = { 100, 500, 1000, 2000, 3000, 4000, 5000, 10000 };
    int numTests = sizeof(testValues) / sizeof(testValues[0]);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(1, 1000000000);

    cout << "n\tHeight\tHeight/log2(n)\n";

    for (int i = 0; i < numTests; i++) {
        int n = testValues[i];
        BST tree;

        for (int j = 0; j < n; j++) {
            tree.insert(dist(gen));
        }

        int h = tree.height();
        double ratio = h / log2((double)n);

        cout << n << "\t" << h << "\t" << ratio << "\n";
    }

    // ========================
    // (b) Delete Function Demo
    // ========================
    BST testTree;
    testTree.insert(50);
    testTree.insert(30);
    testTree.insert(70);
    testTree.insert(20);
    testTree.insert(40);
    testTree.insert(60);
    testTree.insert(80);

    cout << "\nBefore delete, height: " << testTree.height() << endl;

    testTree.remove(50); // delete root (two children case)

    cout << "After delete 50, height: " << testTree.height() << endl;

    return 0;
}
