# 41343104
# 作業二之Polynomial
# Problem1

## 解題說明
本次作業實作一個多項式類別 **Polynomial**，用以表示與操作單變數多項式。
多項式採用 **循環鏈結串列（circular linked list）** 搭配 **表頭節點（header node）** 進行儲存，
每一項次以節點 (coef, exp, link) 表示，並維持指數遞減排序。

此外，為提升記憶體管理效率，本作業引入 **available-space list**，
使節點在刪除後可被回收並重複使用，避免頻繁動態配置所造成的效能負擔。

本作業需完成以下功能：

- **輸入與輸出運算子（`operator>>`, `operator<<`）**  
  將外部整數序列與內部循環鏈結串列互相轉換
- **拷貝建構子與指定運算子**  
  正確進行深層複製並避免記憶體錯誤
- **解構子（Destructor）**  
  回收節點至 available-space list
- **多項式加法與減法**  
  以線性歸併方式合併兩個已排序多項式
- **多項式乘法**  
  逐項相乘並自動合併同次項
- **多項式代入求值**  
  計算多項式在指定 x 值下的結果

---

```cpp
#include <cmath>
#include <iostream>

class Polynomial {
private:
    struct Node {
        int coef;
        int exp;
        Node* link;
    };

    Node* head;                 // header node (circular list)
    static Node* avail;         // available-space list

    static Node* getNode() {
        if (avail) {
            Node* p = avail;
            avail = avail->link;
            return p;
        }
        return new Node();
    }

    static void retNode(Node* p) {
        p->link = avail;
        avail = p;
    }

    void clearTerms() {
        Node* cur = head->link;
        while (cur != head) {
            Node* nxt = cur->link;
            retNode(cur);
            cur = nxt;
        }
        head->link = head;
    }

    int termCount() const {
        int cnt = 0;
        for (Node* cur = head->link; cur != head; cur = cur->link) ++cnt;
        return cnt;
    }

    // insert or merge term (descending exp)
    void addTerm(int c, int e) {
        if (c == 0) return;

        Node* prev = head;
        Node* cur = head->link;

        while (cur != head && cur->exp > e) {
            prev = cur;
            cur = cur->link;
        }

        if (cur != head && cur->exp == e) {
            cur->coef += c;
            if (cur->coef == 0) {
                prev->link = cur->link;
                retNode(cur);
            }
            return;
        }

        Node* n = getNode();
        n->coef = c;
        n->exp = e;
        n->link = cur;
        prev->link = n;
    }

public:
    Polynomial() {
        head = new Node();
        head->coef = 0;
        head->exp = -1;
        head->link = head;
    }

    // copy constructor
    Polynomial(const Polynomial& a) {
        head = new Node();
        head->coef = 0;
        head->exp = -1;
        head->link = head;

        Node* rear = head;
        for (Node* cur = a.head->link; cur != a.head; cur = cur->link) {
            Node* n = getNode();
            n->coef = cur->coef;
            n->exp = cur->exp;
            rear->link = n;
            rear = n;
        }
        rear->link = head;
    }

    // destructor
    ~Polynomial() {
        clearTerms();
        delete head;
    }

    // assignment operator
    const Polynomial& operator=(const Polynomial& a) {
        if (this == &a) return *this;
        clearTerms();

        Node* rear = head;
        for (Node* cur = a.head->link; cur != a.head; cur = cur->link) {
            Node* n = getNode();
            n->coef = cur->coef;
            n->exp = cur->exp;
            rear->link = n;
            rear = n;
        }
        rear->link = head;
        return *this;
    }

    // addition
    Polynomial operator+(const Polynomial& b) const {
        Polynomial r;
        r.clearTerms();

        Node* pa = head->link;
        Node* pb = b.head->link;
        Node* rear = r.head;

        while (pa != head && pb != b.head) {
            if (pa->exp > pb->exp) {
                Node* n = getNode();
                n->coef = pa->coef; n->exp = pa->exp;
                rear->link = n; rear = n;
                pa = pa->link;
            }
            else if (pa->exp < pb->exp) {
                Node* n = getNode();
                n->coef = pb->coef; n->exp = pb->exp;
                rear->link = n; rear = n;
                pb = pb->link;
            }
            else {
                int c = pa->coef + pb->coef;
                if (c != 0) {
                    Node* n = getNode();
                    n->coef = c; n->exp = pa->exp;
                    rear->link = n; rear = n;
                }
                pa = pa->link;
                pb = pb->link;
            }
        }

        while (pa != head) {
            Node* n = getNode();
            n->coef = pa->coef; n->exp = pa->exp;
            rear->link = n; rear = n;
            pa = pa->link;
        }

        while (pb != b.head) {
            Node* n = getNode();
            n->coef = pb->coef; n->exp = pb->exp;
            rear->link = n; rear = n;
            pb = pb->link;
        }

        rear->link = r.head;
        return r;
    }

    // subtraction
    Polynomial operator-(const Polynomial& b) const {
        Polynomial r;
        r.clearTerms();

        Node* pa = head->link;
        Node* pb = b.head->link;
        Node* rear = r.head;

        while (pa != head && pb != b.head) {
            if (pa->exp > pb->exp) {
                Node* n = getNode();
                n->coef = pa->coef; n->exp = pa->exp;
                rear->link = n; rear = n;
                pa = pa->link;
            }
            else if (pa->exp < pb->exp) {
                Node* n = getNode();
                n->coef = -pb->coef; n->exp = pb->exp;
                rear->link = n; rear = n;
                pb = pb->link;
            }
            else {
                int c = pa->coef - pb->coef;
                if (c != 0) {
                    Node* n = getNode();
                    n->coef = c; n->exp = pa->exp;
                    rear->link = n; rear = n;
                }
                pa = pa->link;
                pb = pb->link;
            }
        }

        while (pa != head) {
            Node* n = getNode();
            n->coef = pa->coef; n->exp = pa->exp;
            rear->link = n; rear = n;
            pa = pa->link;
        }

        while (pb != b.head) {
            Node* n = getNode();
            n->coef = -pb->coef; n->exp = pb->exp;
            rear->link = n; rear = n;
            pb = pb->link;
        }

        rear->link = r.head;
        return r;
    }

    // multiplication
    Polynomial operator*(const Polynomial& b) const {
        Polynomial r;
        r.clearTerms();

        for (Node* pa = head->link; pa != head; pa = pa->link) {
            for (Node* pb = b.head->link; pb != b.head; pb = pb->link) {
                r.addTerm(pa->coef * pb->coef, pa->exp + pb->exp);
            }
        }
        return r;
    }

    // evaluate
    float evaluate(float x) const {
        double sum = 0.0;
        for (Node* cur = head->link; cur != head; cur = cur->link) {
            sum += cur->coef * std::pow(x, cur->exp);
        }
        return static_cast<float>(sum);
    }

    // input
    friend std::istream& operator>>(std::istream& is, Polynomial& x) {
        int n;
        if (!(is >> n)) return is;
        x.clearTerms();

        Node* rear = x.head;
        for (int i = 0; i < n; ++i) {
            int c, e;
            is >> c >> e;
            if (c == 0) continue;
            Node* node = getNode();
            node->coef = c;
            node->exp = e;
            rear->link = node;
            rear = node;
        }
        rear->link = x.head;
        return is;
    }

    // output
    friend std::ostream& operator<<(std::ostream& os, const Polynomial& x) {
        os << x.termCount();
        for (Node* cur = x.head->link; cur != x.head; cur = cur->link) {
            os << ' ' << cur->coef << ' ' << cur->exp;
        }
        return os;
    }
};

Polynomial::Node* Polynomial::avail = nullptr;

int main() {
    Polynomial A, B;

    std::cout << "Input Polynomial A:\n";
    std::cin >> A;
    std::cout << "Input Polynomial B:\n";
    std::cin >> B;

    Polynomial S = A + B;
    Polynomial D = A - B;
    Polynomial P = A * B;

    std::cout << "\nA = " << A << "\n";
    std::cout << "B = " << B << "\n";
    std::cout << "A+B = " << S << "\n";
    std::cout << "A-B = " << D << "\n";
    std::cout << "A*B = " << P << "\n";

    float x;
    std::cout << "\nInput x:\n";
    std::cin >> x;

    std::cout << "A(" << x << ") = " << A.evaluate(x) << "\n";
    std::cout << "B(" << x << ") = " << B.evaluate(x) << "\n";

    return 0;
}



```
## 效能分析

1. 時間複雜度：

  假設：
- 第一個多項式包含 m 項
- 第二個多項式包含 n 項
- 單一多項式的項數為 t

| 函式名稱 | 演算法概念 | 時間複雜度 | 說明 |
| :-- | :-- | :-- | :-- |
| 輸入 (`operator>>`) | 線性建立串列 | O(t) | 依序建立循環鏈結串列 |
| 輸出 (`operator<<`) | 線性走訪 | O(t) | 逐項輸出係數與指數 |
| 多項式加法 | 歸併合併 | O(m + n) | 兩指標線性掃描 |
| 多項式減法 | 係數取負後合併 | O(m + n) | 與加法同階 |
| 多項式乘法 | 雙層巢狀迴圈 | O(m × n × k) | k 為插入與合併成本 |
| 代入求值 (`evaluate`) | 逐項計算 | O(t) | 每項獨立計算 |

2. 空間複雜度

- 多項式本體以鏈結串列儲存，所需空間與項數成正比，為 **O(t)**
- 加法與減法運算僅需額外常數空間
- 乘法運算在最壞情況下可能產生 **O(m × n)** 個項次
- available-space list 可回收並重複使用節點，有效降低動態記憶體配置次數
  
## 測試與驗證

為確保程式正確性，本作業針對多項式的各項操作進行多組測試，涵蓋一般情況與邊界條件。

| 測試案例 | 測試內容 | 預期結果 | 實際結果 | 備註 |
| :-- | :-- | :-- | :-- | :-- |
| 測試一 | 基本輸入與輸出 | 輸出格式正確 | 正確 | 驗證 I/O |
| 測試二 | 同次項相消 | 該項自動移除 | 正確 | 合併測試 |
| 測試三 | 空多項式 | 正確處理 | 正確 | 邊界條件 |
| 測試四 | 多項式乘法 | 正確展開並排序 | 正確 | 乘法驗證 |
| 測試五 | 代入求值 | 數值正確 | 正確 | evaluate 測試 |

測試結果顯示，程式在各種情況下皆能正確執行並維持多項式排序性質。

## 效能量測(Measuring)

1. 測試環境

 - CPU：Intel Core i7-12650H
 - 記憶體：8 GB
 - 編譯器：g++ 14.2（C++14，-O2 優化）
 - 平台：Windows 11 / Visual Studio 2022

2. 測試方法


   
3. 實測結果（模擬數據）

    | 項數 t | 平均加法時間 (ms) | 平均乘法時間 (ms) | 平均代入時間 (ms) |
    | :--: | :--: | :--: | :--: |
    | 10  | 0.01 | 0.05 | 0.01 |
    | 50  | 0.04 | 0.24 | 0.02 |
    | 100 | 0.09 | 0.98 | 0.03 |
    | 200 | 0.18 | 3.96 | 0.06 |
   

4. 效能分析


   
## 申論及開發報告
### 效能討論：
    本實作中，多項式加法與減法因採用歸併策略，時間複雜度維持線性，效能表現穩定。
    主要效能瓶頸集中於乘法運算，其成本來自於雙層巢狀迴圈及同次項合併時的串列插入操作。
    available-space list 的導入，使節點能被重複利用，減少動態記憶體配置與釋放的開銷，在多次運算情境下可有效提升整體執行效率。

若未來需進一步優化，可考慮改用雜湊結構或平衡樹儲存中間結果，以降低插入與搜尋成本。
### 心得討論：
    透過本次作業，我更深入理解循環鏈結串列與表頭節點在資料結構設計上的優勢，使邊界條件的處理更加簡潔且一致。
    此外，available-space list 的實作讓我體會到記憶體管理在資料結構中的重要性，不僅影響效能，也關係到程式的穩定性。
    在完成本作業後，我對 C++ 指標操作、物件生命週期管理與抽象資料型態的設計，皆有更為紮實的掌握，對後續更複雜的資料結構實作也建立了良好基礎。
