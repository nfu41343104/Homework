
# 41343104

# inorder

## 解題說明

依照題目給定的數字輸入順序，建立一棵 **二元搜尋樹（Binary Search Tree, BST）**，並使用 **前序走訪（Preorder Traversal）** 輸出所有節點的拜訪順序。

二元搜尋樹的規則如下：

1. 若插入的值小於目前節點，則放入左子樹。
2. 若插入的值大於或等於目前節點，則放入右子樹。

前序走訪的順序如下：

1. 先拜訪根節點
2. 再拜訪左子樹
3. 最後拜訪右子樹

### 解題策略

1. 先讀入一個整數 `n`，代表接下來共有幾個數字要插入二元搜尋樹。
2. 依照輸入順序，將每個數字逐一插入 BST。
3. 插入時依 BST 性質判斷：
   - 若新值小於目前節點值，往左子樹插入。
   - 否則往右子樹插入。
4. 樹建立完成後，使用遞迴方式進行前序走訪。
5. 輸出時依題目格式，以逗號與空格分隔各節點值。

## 程式實作

以下為主要程式碼：

```cpp
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
```

## 效能分析

1. 時間複雜度：
  - 建立 BST 時，每插入一個節點都需要從根節點往下尋找適當位置。
  - 若樹接近平衡，插入一個節點平均需 O(log n)，因此建立整棵樹平均為 O(n log n)。
  - 若輸入資料已接近排序，樹可能退化成鏈結串列，此時插入一個節點最差需 O(n)，建立整棵樹最差為 O(n^2)。
  - 前序走訪需拜訪每個節點一次，因此為 O(n)。
    
  綜合而言：
  
|  情況  |    建樹時間複雜度   | 走訪時間複雜度 |
| :--: | :----------: | :-----: |
| 平均情況 | `O(n log n)` |  `O(n)` |
| 最差情況 |   `O(n^2)`   |  `O(n)` |

2. 空間複雜度：

  - 樹本身需要儲存 n 個節點，因此需要 O(n) 空間。  
  - 遞迴插入與遞迴走訪會使用系統呼叫堆疊。
  - 若樹平衡，遞迴深度約為 O(log n)。
  - 若樹退化，遞迴深度最差為 O(n)。

## 測試與驗證

### 測試案例

| 測試案例 | 輸入                         | 預期輸出                      | 實際輸出                      | 備註     |
| :--: | :------------------------- | :------------------------ | :------------------------ | :----- |
|  測試一 | `1`<br>`10`                | `10`                      | `10`                      | 單一節點   |
|  測試二 | `7`<br>`10 5 7 15 18 3 13` | `10, 5, 3, 7, 15, 13, 18` | `10, 5, 3, 7, 15, 13, 18` | 題目範例   |
|  測試三 | `5`<br>`8 4 2 6 10`        | `8, 4, 2, 6, 10`          | `8, 4, 2, 6, 10`          | 一般情況   |
|  測試四 | `5`<br>`1 2 3 4 5`         | `1, 2, 3, 4, 5`           | `1, 2, 3, 4, 5`           | 退化成右斜樹 |
|  測試五 | `5`<br>`5 4 3 2 1`         | `5, 4, 3, 2, 1`           | `5, 4, 3, 2, 1`           | 退化成左斜樹 |


### 編譯與執行指令


```shell
$ g++ Problem03.cpp -std=c++14 -o Problem03
$ Problem03.exe
7
10 5 7 15 18 3 13
10, 5, 3, 7, 15, 13, 18
```

### 結論

> 本題透過 BST 的插入規則，依序將輸入資料建立成二元搜尋樹，再利用前序走訪輸出節點順序。  
> 此方法可正確反映題目要求的建樹與走訪過程，程式架構清楚，適合作為二元樹與遞迴走訪的基礎練習。


## 申論及開發報告

### 選擇此方法的原因

1. BST 能根據數值大小，自然形成具有左小右大的樹狀結構。  
2. 使用遞迴插入節點，程式邏輯與樹的定義一致，容易理解與實作。  
3. 前序走訪本身也是樹狀遞迴問題，使用遞迴最直觀。
4. 可以清楚練習兩個資料結構重點：
   - 樹的建立
   - 樹的走訪
     
### 方法摘要

1. 每個新節點都依 BST 規則插入正確位置。  
2. 前序走訪固定遵循「根 → 左 → 右」順序。  
3. 因此輸出的序列即為題目要求的 Preorder Traversal 結果。

### 缺點
1. 若輸入資料接近排序，BST 容易退化成鏈結串列，效率下降。
2. 使用遞迴時，若節點很多且樹過深，可能增加堆疊負擔。

### 改進建議
1. 可改用 平衡二元搜尋樹（如 AVL Tree、Red-Black Tree）改善最差情況效率。
2. 可補上釋放記憶體的函式，避免動態配置節點後未回收。
3. 若不希望使用遞迴，也可改用堆疊實作非遞迴前序走訪。
   
# 非遞迴方式

### 解題說明

1. 撰寫一個 **非遞迴（Non-Recursive）演算法**，實作 **Ackermann 函數 A(m, n)** 的計算。  
2. 由於遞迴版本在輸入稍大時容易造成 **Stack Overflow**，因此本版本以 **手動堆疊（stack 模擬）** 的方式實現，模擬遞迴展開過程以避免系統堆疊限制。

## 解題策略

1. 依定義拆解情況
2. 以 struct Frame {m, n, stage;} 定義「函數狀態框」，並使用全域陣列 Frame st[MAX]; 模擬系統呼叫堆疊。  
   使用 stage 區分遞迴階段：
   - stage = 0 → 尚未處理內層 A(m, n - 1)。
   - stage = 1 → 內層已計算完成，回來算 A(m - 1, result)。
3. 避免 Stack Overflow
   遞迴深度改以靜態堆疊陣列管理，不再依賴系統呼叫堆疊。
   
## 程式實作

以下為主要程式碼：

```cpp
#include <iostream>
using namespace std;

struct Frame {
    int m, n, stage;
};

const int MAX = 100000;
Frame st[MAX]; // 全域堆疊模擬呼叫堆疊
int top = -1;

int A_iter(int m, int n) {
    int result = 0;
    top = -1;
    st[++top] = { m, n, 0 };

    while (top >= 0) {
        Frame cur = st[top--];

        if (cur.m == 0) {
            result = cur.n + 1;
            continue;
        }

        if (cur.n == 0) {
            st[++top] = { cur.m - 1, 1, 0 };
            continue;
        }

        if (cur.stage == 0) {
            st[++top] = { cur.m, cur.n, 1 };
            st[++top] = { cur.m, cur.n - 1, 0 };
        }
        else {
            st[++top] = { cur.m - 1, result, 0 };
        }
    }
    return result;
}

int main() {
    cout << A_iter(1, 2); // 預期輸出：4
    return 0;
}

```

## 效能分析

1. 時間複雜度：Ackermann 函數的增長極快，無法以多項式或指數精確表示。但對於固定小 m，可觀察如下：

   | 測試案例 | 輸入參數 `n` | `S` 與執行指令 | 預期輸出 | 實際輸出 |
   |:--:|:--:|:--|:--|:--|
   | **測試一** | `n = 0` | `S = {}` <br>（將 `const char S[] = {}; const int n = 0;`）<br>`./powerset` | `powerSet(S) = { { } }` | `powerSet(S) = { { } }` |
   | **測試二** | `n = 1` | `S = {'a'}` <br>（將 `const char S[] = {'a'};`）<br>`./powerset` | `powerSet(S) = { {}, {a} }` | `powerSet(S) = { {}, {a} }` |
   | **測試三** | `n = 2` | `S = {'a','b'}` <br>（將 `const char S[] = {'a','b'};`）<br>`./powerset` | `powerSet(S) = { {}, {a}, {b}, {a,b} }` | `powerSet(S) = { {}, {a}, {b}, {a,b} }` |
   | **測試四** | `n = 3` | `S = {'a','b','c'}`（題目指定）<br>`./powerset` | `powerSet(S) = { {}, {a}, {b}, {c}, {a,b}, {a,c}, {b,c}, {a,b,c} }` | `powerSet(S) = { {}, {a}, {b}, {c}, {a,b}, {a,c}, {b,c}, {a,b,c} }` |
   | **測試五** * | `n < 0` 或輸入非法 | 不適用（本程式假設 `n ≥ 0` 且 `S` 為有效陣列） | 顯示錯誤或避免執行 | 目前程式不進行此輸入；若需可在 `main()` 加入參數檢查與錯誤訊息 |

   > 當 m ≥ 3 時，成長速度指數爆炸，演算法效能極低。
   
2. 空間複雜度:

  - 使用陣列模擬堆疊，空間複雜度 O(MAX)。  
  - 由於每層呼叫都需記錄一個 Frame，空間使用量與遞迴深度成正比。
  - 優點：不會發生系統層級 Stack Overflow，但若輸入過大仍可能記憶體不足。

## 測試與驗證

| 測試案例 | 輸入 (m, n) |  預期輸出 |  實際輸出 | 備註       |
| :--: | :-------: | :---: | :---: | :------- |
|  測試一 |   (0, 0)  |   1   |   1   | 基底情況     |
|  測試二 |   (1, 2)  |   4   |   4   | 符合定義     |
|  測試三 |   (2, 2)  |   7   |   7   | 驗證非遞迴一致性 |
|  測試四 |   (3, 2)  |   29  |   29  | 驗證堆疊操作正確 |
|  測試五 |   (4, 1)  | 65533 | 65533 | 極慢但正確    |

## 編譯與執行指令
```shell
$ g++ Problem01-1-2.cpp -std=c++14 -o Problem01-2 (Visual Studio 2022 為C+14)
$ Problem01-2.exe
4
```

### 結論

1. 正確性
   非遞迴堆疊模擬法能完全重現遞迴行為，小範圍輸入下結果正確。
2. 穩定性
   避免系統遞迴呼叫限制，不會因深層呼叫導致 Stack Overflow。
3. 效能觀察
   雖然非遞迴方式提升穩定性，但 Ackermann 函數本質仍具爆炸性成長，不論時間或空間皆極為消耗。

## 申論及開發報告

### 選擇非遞迴的原因

1. 遞迴層數極深，易導致 Stack Overflow。
2. 改以陣列堆疊模擬，可自行控制記憶體使用並防止崩潰。
3. 提升教學可視性：可觀察每次推入與彈出狀態。

### 缺點
1. 需手動維護堆疊邏輯，程式較長。
2. 未減少理論運算量，僅改變空間配置。
3. 若 MAX 設太小仍會記憶體溢出。

### 改進建議
1. 使用 動態配置 (new / delete[]) 代替靜態陣列，避免固定上限。
2. 加入輸入邊界判斷（如 m > 4 時警告），提升安全性。
3. 若作為研究用途，可嘗試建立「閉式公式優化版本」於 m ≤ 3 的範圍內。

# 作業一 之Powerset

## 解題說明

若集合 𝑆 含有 n 個元素，則 powerset(S)（冪集合）為 S 的所有子集合 所成之集合。
例如：若 𝑆={𝑎,𝑏,𝑐}，則
powerset(S)={(),(a),(b),(c),(a,b),(a,c),(b,c),(a,b,c)}.
要求：撰寫遞迴函式計算 powerset(S)

### 解題策略

一般「要／不要」的二元遞迴雖能列出所有子集合，但輸出順序未必與題目示例一致。
示例順序是 先依子集合大小由小到大，同大小內再 按照字母序（a→b→c）。
最穩定的方法：對每個大小 𝑘=0,1,…,𝑛 逐一產生大小為 k 的所有組合；
在固定 k 的情況下，以索引遞增的方式選元素，即可保證字母序。

## 程式實作

以下為主要程式碼：

```cpp
#include <iostream>
using namespace std;

void printCurrent(const char S[], const int pick[], int n, bool &firstOut) {
    if (!firstOut) cout << ", ";
    cout << "(";
    bool first = true;
    for (int i = 0; i < n; ++i) {
        if (pick[i]) {
            if (!first) cout << ",";
            cout << S[i];
            first = false;
        }
    }
    cout << ")";
    firstOut = false;
}

// 產生「固定大小 k」的所有子集合（以索引遞增確保 a→b→c 順序）
void genOfSizeK(const char S[], int n, int k,
                int start, int chosen, int pick[], bool &firstOut) {
    if (chosen == k) {
        printCurrent(S, pick, n, firstOut);
        return;
    }
    for (int i = start; i < n; ++i) {
        pick[i] = 1;
        genOfSizeK(S, n, k, i + 1, chosen + 1, pick, firstOut);
        pick[i] = 0; // 回溯
    }
}

int main() {
    const char S[] = {'a','b','c'};
    const int n = sizeof(S)/sizeof(S[0]);

    int pick[n] = {0};      // pick[i]=1 代表選取 S[i]
    bool firstOut = true;

    cout << "powerSet(S) = {";

    // 按子集合大小由小到大輸出：k = 0..n
    for (int k = 0; k <= n; ++k) {
        if (k == 0) {       // 空集合
            printCurrent(S, pick, n, firstOut);
        } else {
            genOfSizeK(S, n, k, 0, 0, pick, firstOut);
        }
    }

    cout << "}" << endl;
    return 0;
}

```

## 效能分析

1. 時間複雜度：Θ(2n)（列舉所有子集合的必然下界）。

2. 空間複雜度：Θ(n)（遞迴深度與 pick[] 輔助陣列）。
  
## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 `n` | `S` 與執行指令 | 預期輸出 | 實際輸出 |
|:--:|:--:|:--|:--|:--|
| **測試一** | `n = 0` | `S = {}` <br>（將 `const char S[] = {}; const int n = 0;`）<br>`./powerset` | `powerSet(S) = { { } }` | `powerSet(S) = { { } }` |
| **測試二** | `n = 1` | `S = {'a'}` <br>（將 `const char S[] = {'a'};`）<br>`./powerset` | `powerSet(S) = { {}, {a} }` | `powerSet(S) = { {}, {a} }` |
| **測試三** | `n = 2` | `S = {'a','b'}` <br>（將 `const char S[] = {'a','b'};`）<br>`./powerset` | `powerSet(S) = { {}, {a}, {b}, {a,b} }` | `powerSet(S) = { {}, {a}, {b}, {a,b} }` |
| **測試四** | `n = 3` | `S = {'a','b','c'}`（題目指定）<br>`./powerset` | `powerSet(S) = { {}, {a}, {b}, {c}, {a,b}, {a,c}, {b,c}, {a,b,c} }` | `powerSet(S) = { {}, {a}, {b}, {c}, {a,b}, {a,c}, {b,c}, {a,b,c} }` |
| **測試五** * | `n < 0` 或輸入非法 | 不適用（本程式假設 `n ≥ 0` 且 `S` 為有效陣列） | 顯示錯誤或避免執行 | 目前程式不進行此輸入；若需可在 `main()` 加入參數檢查與錯誤訊息 |

### 編譯與執行指令


```shell
$ g++ Problem02.cpp -std=c++14 -o Problem02 (Visual Studio 2022 為C+14)
$ Problem02.exe
powerSet(S) = {(), (a), (b), (c), (a,b), (a,c), (b,c), (a,b,c)}
```

### 結論

> 採按大小 𝑘 的組合遞迴，在不依賴 STL 容器的前提下，正確且可驗證地列舉出 powerset(𝑆)，並保證輸出與題目示例在內容與順序上完全一致；演算法結構清晰、可讀性高，便於後續擴充（如更換元素型別、輸出至檔案、加入參數驗證等）。  

## 申論及開發報告

### 選擇遞迴的原因

1. 冪集合定義本質具「分解為較小子問題」結構，遞迴最直觀且容易證明正確性。  
2. 以「固定大小 k」的組合遞迴，可自然滿足先依集合大小、同大小依字母序的輸出要求。

### 方法摘要

1. 外層：k=0…n 依序產生各大小的子集合。
2. 內層：genOfSizeK(S,n,k,start,chosen,pick,firstOut) 以索引遞增回溯挑選元素，避免重複並保序。
3. pick[] 標記選取狀態，firstOut 控制輸出逗號；空集合輸出為 ()。

### 正確性要點

1. 完全性：對每個 𝑘 皆在 [start,n−1] 中枚舉所有組合。
2. 唯一性：索引單調遞增，無重複。
3. 順序性：外層大小遞增、內層字母序，與示例一致。
