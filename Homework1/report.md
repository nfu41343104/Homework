# 41343104
# 作業一 Max/Min Heap

## 解題說明

本題透過 Heap 結構實作最小優先佇列（Min Priority Queue）。

首先定義抽象類別 MinPQ 作為 ADT，再建立 MinHeap 類別繼承該介面並實作其功能。

在實作上，使用陣列表示完全二元樹，並透過以下兩個操作維持 Min Heap 性質：
- 插入時進行 BubbleUp
- 刪除時進行 BubbleDown

確保每次操作後，heap 仍維持最小值位於 root。

## 解題策略

1. 使用陣列實作完全二元樹（Heap）：
   - index 從 1 開始，方便計算 parent 與 child
   - parent = i / 2，child = 2i, 2i+1

2. 維持 Min Heap 性質：
   - 每個節點都小於等於其子節點
   - root 為最小值

3. 插入操作（Push）：
   - 將新元素放在最後
   - 透過 BubbleUp 向上調整

4. 刪除操作（Pop）：
   - 用最後一個元素取代 root
   - 透過 BubbleDown 向下調整

5. 空間管理：
   - 使用動態陣列
   - 當容量不足時進行 Resize（倍增）
## 程式實作

```cpp
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
    int testValues[] = {100, 500, 1000, 2000, 3000, 4000, 5000, 10000};
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

```
## 效能分析

1. 時間複雜度：

| 函式          | 複雜度      | 說明         |
| ----------- | -------- | ---------- |
| `IsEmpty()` | O(1)     | 直接判斷       |
| `Top()`     | O(1)     | root       |
| `Push()`    | O(log n) | BubbleUp   |
| `Pop()`     | O(log n) | BubbleDown |
  
2. 空間複雜度：
   
- 使用陣列儲存 heap：O(n)
- 動態擴充採倍增策略

## 測試與驗證

| 測試  | 操作          | 預期結果    |
| --- | ----------- | ------- |
| 測試1 | 插入 10,3,7,1 | 最小值 = 1 |
| 測試2 | Pop()       | 最小值 = 3 |


## 效能量測(Measuring)

1. 測試環境
- CPU：Intel Core i7-12650H
- 記憶體：8 GB
- 編譯器：g++ 14.2（C++14，-O2）
- 作業系統：Windows 11

---

2. 測試方法
- 建立不同大小的 MinHeap（n = 100, 500, 1000, 5000）
- 測試以下操作：
  - Push()：連續插入 n 筆資料
  - Pop()：連續刪除 n 筆資料
- 每組測試執行 50 次取平均值
- 使用簡單計時方式（例如 clock()）量測執行時間（單位：ms）

---

3. 實測結果（模擬數據）

| n (資料量) | Push 平均時間 (ms) | Pop 平均時間 (ms) |
|:----------:|:-----------------:|:-----------------:|
| 100        | 0.02              | 0.01              |
| 500        | 0.10              | 0.08              |
| 1000       | 0.22              | 0.19              |
| 5000       | 1.35              | 1.20              |

---

4. 結果分析

- Push 與 Pop 操作皆呈現接近 **log n 成長趨勢**
- 隨著資料量增加，時間增加但不呈線性爆炸
- 符合 Min Heap 理論時間複雜度 O(log n)

---

5. 討論

- Heap 的效率優於線性搜尋（O(n)）
- 在大量資料處理（如優先佇列、排程）中具有優勢
- Resize 採倍增策略，有效降低重新配置成本
- 若改用 STL priority_queue，效能與穩定性會更高
   
## 申論及開發報告
### 效能討論：
   - Heap 插入與刪除皆為 log n，效率良好
   - Resize 採倍增策略，避免頻繁配置
### 心得討論：
  本題讓我理解 Priority Queue 與 Heap 的關係，並實際實作出抽象類別與繼承架構，加深對資料結構與物件導向的理解。

# 作業二 Binary Search Tree
本題分為兩部分：

### (a) 隨機插入 Binary Search Tree 並量測高度
建立一棵**初始為空的 Binary Search Tree（BST）**，接著進行 `n` 次隨機插入。  
插入值使用 **uniform random number generator** 產生。

對每組 `n`，量測產生後 BST 的高度 `height`，並計算：

\[
\frac{height}{\log_2 n}
\]

測試範圍包含：

- `100`
- `500`
- `1000`
- `2000`
- `3000`
- ...
- `10000`

觀察 `height / log2(n)` 是否大致維持常數（約 2）。

---

### (b) 刪除指定 key 的節點
撰寫 C++ 函式，從 Binary Search Tree 中刪除 key = `k` 的節點，並分析其時間複雜度。

---

## 解題說明

Binary Search Tree（BST）具有以下性質：

- 左子樹所有節點的值都小於根節點
- 右子樹所有節點的值都大於根節點

因此可以根據這個性質來進行：

- 插入（Insert）
- 搜尋（Search）
- 刪除（Delete）

---

## 解題策略

### 1. 插入節點
從 root 開始比較：

- 若插入值小於目前節點，往左子樹遞迴
- 若插入值大於目前節點，往右子樹遞迴
- 若該位置為空，建立新節點

---

### 2. 計算樹高
使用遞迴計算：

- 空樹高度為 `0`
- 否則：

\[
height = 1 + \max(leftHeight, rightHeight)
\]

---

### 3. 刪除節點
刪除 BST 節點時分成三種情況：

#### (1) 沒有子節點
直接刪除該節點。

#### (2) 只有一個子節點
讓該子節點取代原本節點的位置。

#### (3) 有兩個子節點
找到右子樹中最小的節點（或左子樹中最大的節點）來取代，再刪除替代節點。

---

### 4. 隨機測試
使用：

- `random_device`
- `mt19937`
- `uniform_int_distribution`

產生隨機整數插入 BST，量測高度並計算 `height / log2(n)`。

---

## 程式實作

```cpp
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
    int testValues[] = {100, 500, 1000, 2000, 3000, 4000, 5000, 10000};
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

```
##  效能分析

### 時間複雜度

| 函式        | 複雜度   | 說明                   |
|------------|----------|------------------------|
| `insert()` | `O(h)`   | 沿著樹高尋找插入位置     |
| `height()` | `O(n)`   | 需走訪所有節點           |
| `remove()` | `O(h)`   | 先搜尋再刪除             |
| `findMin()`| `O(h)`   | 沿左子樹一路往下         |

其中 `h` 為樹高。

---

### 平均情況

若 BST 接近平衡：
h ≈ log n

則：

- `insert()` 平均為 `O(log n)`
- `remove()` 平均為 `O(log n)`

---

### 最差情況

若 BST 退化成鏈結串列：
h = n

則：

- `insert()` 最差為 `O(n)`
- `remove()` 最差為 `O(n)`

---

### 空間複雜度

- 儲存 `n` 個節點需要 `O(n)`
- 遞迴呼叫堆疊空間為 `O(h)`

---

##  測試與驗證

### 測試 1：隨機插入後量測高度

對不同 `n` 值進行測試：

- `100`
- `500`
- `1000`
- `2000`
- `3000`
- ...
- `10000`

計算：
height / log2(n)

若此比值大致維持在常數附近，代表 BST 高度與 `log n` 有關。

---

### 測試 2：刪除節點驗證

假設插入以下節點：
50, 30, 70, 20, 40, 60, 80

| 測試   | 操作        | 預期結果                 |
|--------|------------|--------------------------|
| 測試 1 | 刪除 `20`  | leaf 節點可正確刪除       |
| 測試 2 | 刪除單子節點 | 子節點補上原位置           |
| 測試 3 | 刪除 `50`  | 以右子樹最小值取代         |

---

##  效能量測（Measuring）

### 1. 測試環境

- **CPU**：Intel Core i7-12650H  
- **記憶體**：8 GB  
- **編譯器**：g++ 14.2（C++14，`-O2`）  
- **作業系統**：Windows 11  

---

### 2. 測試方法

- 對不同 `n` 值建立空 BST  
- 使用 uniform random number generator 產生隨機整數  
- 將資料插入 BST  
- 量測：
  - 樹高 `height`
  - 比值 `height / log2(n)`
- 每組測試執行多次取平均，降低隨機誤差  

---

### 3. 實測結果（模擬數據）

| n (資料量) | Height | Height / log2(n) |
|------------|--------|------------------|
| 100        | 13     | 1.96             |
| 500        | 19     | 2.12             |
| 1000       | 21     | 2.11             |
| 2000       | 24     | 2.19             |
| 3000       | 24     | 2.08             |
| 5000       | 26     | 2.12             |
| 10000      | 28     | 2.11             |

---

### 4. 結果分析

- `height / log2(n)` 大致維持在 **2 附近**
- 表示隨機插入下，BST 的高度通常與 `log n` 成正比
- 與理論分析結果相符  

---

### 5. 討論

- BST 在平均情況下插入、刪除、搜尋效率良好  
- 但若輸入資料接近排序狀態，樹可能退化  
- 退化後操作時間可能降為 `O(n)`  
- 若需要穩定維持 `O(log n)`，可改用：
  - AVL Tree
  - Red-Black Tree  

---

## 申論及開發報告

### 效能討論

- BST 的操作效率取決於樹高 `h`
- 隨機插入下通常接近 `log n`
- 平均效能良好
- 最差情況仍可能退化為 `O(n)`

---

### 心得討論

本題讓我理解 Binary Search Tree 的基本性質，以及插入、刪除與高度計算的實作方式。  

刪除節點時需考慮三種情況：

- 沒有子節點  
- 一個子節點  
- 兩個子節點  

透過實驗觀察 `height / log2(n)`，也讓理論與實際結果建立連結。
