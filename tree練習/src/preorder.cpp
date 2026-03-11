#include <iostream>
using namespace std;

struct Node {
    int data;
    Node* left;
    Node* right;

    Node(int val) {
        data = val;
        left = NULL;
        right = NULL;
    }
};

Node* insert(Node* root, int val) {
    if (root == NULL) {
        return new Node(val);
    }

    if (val < root->data) {
        root->left = insert(root->left, val);
    }
    else {
        root->right = insert(root->right, val);
    }

    return root;
}

void preorder(Node* root, bool& first) {
    if (root == NULL) return;

    if (!first) cout << ", ";
    cout << root->data;
    first = false;

    preorder(root->left, first);
    preorder(root->right, first);
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
    preorder(root, first);

    return 0;
}
