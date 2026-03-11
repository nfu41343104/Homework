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
    }
    else {
        root->right = insert(root->right, value);
    }

    return root;
}

void levelOrder(Node* root, int n) {
    if (root == NULL) return;

    Node** q = new Node * [n];
    int front = 0, rear = 0;

    q[rear++] = root;
    bool first = true;

    while (front < rear) {
        Node* cur = q[front++];

        if (!first) cout << ", ";
        cout << cur->data;
        first = false;

        if (cur->left != NULL) q[rear++] = cur->left;
        if (cur->right != NULL) q[rear++] = cur->right;
    }

    delete[] q;
}

int main() {
    int n;
    cin >> n;

    Node* root = NULL;

    for (int i = 0; i < n; i++) {
        int x;
        cin >> x;
        root = insert(root, x);
    }

    levelOrder(root, n);

    return 0;
}
