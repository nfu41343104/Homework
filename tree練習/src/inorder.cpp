#include <iostream>
using namespace std;

struct Node {
    int data;
    Node* left;
    Node* right;

    Node(int val) {
        data = val;
        left = nullptr;
        right = nullptr;
    }
};

// 插入 BST
Node* insert(Node* root, int val) {
    if (root == nullptr) {
        return new Node(val);
    }

    if (val < root->data) {
        root->left = insert(root->left, val);
    } else {
        root->right = insert(root->right, val);
    }

    return root;
}

// 前序走訪
void preorder(Node* root, bool &first) {
    if (root == nullptr) return;

    if (!first) cout << ", ";
    cout << root->data;
    first = false;

    preorder(root->left, first);
    preorder(root->right, first);
}

int main() {
    int n;
    cin >> n;

    Node* root = nullptr;

    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        root = insert(root, x);
    }

    bool first = true;
    preorder(root, first);
    cout << endl;

    return 0;
}
