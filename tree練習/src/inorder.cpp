#include <iostream>
using namespace std;

struct Node {
    int data;
    Node* left;
    Node* right;

    Node(int v) {
        data = v;
        left = NULL;
        right = NULL;
    }
};

Node* insert(Node* root, int value) {
    if (root == NULL) {
        return new Node(value);
    }

    if (value < root->data) {
        root->left = insert(root->left, value);
    } else {
        root->right = insert(root->right, value);
    }

    return root;
}

void inorder(Node* root, bool& first) {
    if (root == NULL) return;

    inorder(root->left, first);

    if (!first) {
        cout << ", ";
    }
    cout << root->data;
    first = false;

    inorder(root->right, first);
}

int main() {
    int n, x;
    cin >> n;

    Node* root = NULL;

    for (int i = 0; i < n; i++) {
        cin >> x;
        root = insert(root, x);
    }

    bool first = true;
    inorder(root, first);
    cout << endl;

    return 0;
}
