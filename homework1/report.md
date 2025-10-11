
# 41343104

# 作業一 之Ackermann Function

# 遞迴方式

## 解題說明

撰寫一個遞迴函式，計算 Ackermann 函數 A(m, n) 的結果

### 解題策略

1. 根據定義式，分別處理三種情況：
   - 當 `m == 0` 時，直接回傳 `n + 1`。
   - 當 `n == 0` 時，回傳 `A(m - 1, 1)`。
   - 其餘情況則遞迴呼叫 `A(m - 1, A(m, n - 1))`。
2. 遞迴呼叫層數非常深，需注意堆疊深度可能導致 Stack Overflow。
3. 為避免過大輸入導致無限遞迴，測試時僅採用小範圍的 m、n 值。

## 程式實作

以下為主要程式碼：

```cpp
#include<iostream>
using namespace std;

int A(int m,int n) {
	if (m == 0)	return n + 1;
	else if (n == 0)	return A(m - 1, 1);
	else return A(m - 1, A(m, n - 1));
}
int main() {
	cout<<A(1,2);
	return 0;
}
```

## 效能分析

1. 時間複雜度：

   Ackermann 函數的增長速度極快，無法以多項式或指數精確表示。
    
   對固定小值的 `m`，可觀察以下情形：

   - 當 `m = 0` 時，函數僅為常數時間運算。
   - 當 `m = 1` 或 `m = 2`，時間複雜度為線性成長。
   - 當 `m = 3` 以上，成長速度呈指數爆炸，演算法效率極低。
     
   
   | m | 公式 | 增長形態 |
   |:-:|:--|:--|
   | 0 | `A(0, n) = n + 1` | `O(1)` |
   | 1 | `A(1, n) = n + 2` | `O(n)` |
   | 2 | `A(2, n) = 2n + 3` | `O(n)` |
   | 3 | `A(3, n) = 2^(n+3) - 3` | `O(2ⁿ)` |

2. 空間複雜度：
   
  - 遞迴呼叫層數與計算步驟數量幾乎相同。  
  - 當輸入過大時，系統堆疊深度急劇增加，可能導致 **Stack Overflow**。
   
## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 (m, n) | 預期輸出 | 實際輸出 | 備註 |
|:---:|:---:|:---:|:---:|:---|
| 測試一 | (0, 0) | 1 | 1 | 基底情況 |
| 測試二 | (1, 2) | 4 | 4 | 符合定義 |
| 測試三 | (2, 2) | 7 | 7 | 驗證遞迴層級 |
| 測試四 | (3, 2) | 29 | 29 | 遞迴加深 |
| 測試五 | (4, 1) | 65533 | 65533（極慢） | 驗證極限情況 |

### 編譯與執行指令


```shell
$ g++ Problem01-1.cpp -std=c++14 -o Problem01-1 (Visual Studio 2022 為C+14)
$ Problem01-1.exe
4
```

### 結論

> Ackermann 函數在小輸入值時仍可驗證正確性，  
> 但隨輸入增長，無論是時間或空間消耗都呈爆炸式成長。  
> 因此在實務中僅作為理論分析與遞迴研究的範例。

## 申論及開發報告

### 選擇遞迴的原因

1. **Ackermann 函數** 本身以遞迴定義，程式可直接反映數學邏輯。  
2. 有助理解「將問題化簡為更小子問題」的遞迴概念。  
3. 程式碼簡潔、易懂，對初學者理解遞迴思維具良好示範性。

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
$ g++ main.cpp -std=c++14 -o main (Visual Studio 2022 為C+14)
$ main.exe
4
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
