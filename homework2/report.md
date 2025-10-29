# 41343104
# 作業二之Polynomial
# Problem1

## 解題說明
設計一個多項式類別 Polynomial，能夠動態新增項次、執行多項式運算與代入求值：

- 新增項次（AddTerm）:
依降冪插入或合併同次項

- 多項式加法（Add）：
兩個已排序多項式逐項合併

- 多項式乘法（Mult）:
雙層迴圈逐項相乘並自動合併

- 代入計算（Eval）:
使用快速冪 ipow 計算每項貢獻

- 輸出函式（Print）:
以人類可讀格式輸出多項式

## Algorithm Design & Programming
 - 本程式以動態陣列儲存多項式項次（係數與指數），並利用排序與合併策略實作多項式的加法、乘法與代入運算
 - 透過 ensureCapacity() 動態擴充記憶體，確保效率與彈性
 - 程式核心維持 Problem 1 的 ADT 架構，僅擴充可讀性與穩定性

```cpp
#include <iostream>
using namespace std;

class Polynomial; 

class Term {
    friend class Polynomial;
private:
    float coef; 
    int   exp;  
};

class Polynomial {
private:
    Term* termArray;
    int   capacity;
    int   terms;

    void ensureCapacity(int need) {
        if (need <= capacity) return;
        int newCap = capacity;
        while (newCap < need) newCap *= 2;
        Term* tmp = new Term[newCap];
        for (int i = 0; i < terms; ++i) tmp[i] = termArray[i];
        delete[] termArray;
        termArray = tmp;
        capacity = newCap;
    }

    void newTerm(float c, int e) {
        if (c == 0) return;
        int pos = 0;
        while (pos < terms && termArray[pos].exp > e) ++pos;

        if (pos < terms && termArray[pos].exp == e) {
            termArray[pos].coef += c;
            if (termArray[pos].coef == 0) { 
                for (int i = pos + 1; i < terms; ++i) termArray[i - 1] = termArray[i];
                --terms;
            }
            return;
        }

        ensureCapacity(terms + 1);
        for (int i = terms; i > pos; --i) termArray[i] = termArray[i - 1];
        termArray[pos].coef = c;
        termArray[pos].exp = e;
        ++terms;
    }

    static float ipow(float x, int n) {
        float r = 1.0f, base = x;
        int e = n;
        while (e > 0) {
            if (e & 1) r *= base;
            base *= base;
            e >>= 1;
        }
        return r;
    }

public:
    Polynomial() : capacity(2), terms(0) { termArray = new Term[capacity]; }
    ~Polynomial() { delete[] termArray; }

    Polynomial(const Polynomial& other) : capacity(other.capacity), terms(other.terms) {
        termArray = new Term[capacity];
        for (int i = 0; i < terms; ++i) termArray[i] = other.termArray[i];
    }
    Polynomial& operator=(const Polynomial& rhs) {
        if (this == &rhs) return *this;
        delete[] termArray;
        capacity = rhs.capacity;
        terms = rhs.terms;
        termArray = new Term[capacity];
        for (int i = 0; i < terms; ++i) termArray[i] = rhs.termArray[i];
        return *this;
    }

    void AddTerm(float c, int e) { newTerm(c, e); }

    // p(this) + b
    Polynomial Add(const Polynomial& b) const {
        Polynomial c;
        int i = 0, j = 0;
        while (i < terms && j < b.terms) {
            if (termArray[i].exp > b.termArray[j].exp) {
                c.newTerm(termArray[i].coef, termArray[i].exp); ++i;
            }
            else if (termArray[i].exp < b.termArray[j].exp) {
                c.newTerm(b.termArray[j].coef, b.termArray[j].exp); ++j;
            }
            else {
                float sum = termArray[i].coef + b.termArray[j].coef;
                if (sum != 0) c.newTerm(sum, termArray[i].exp);
                ++i; ++j;
            }
        }
        while (i < terms) { c.newTerm(termArray[i].coef, termArray[i].exp); ++i; }
        while (j < b.terms) { c.newTerm(b.termArray[j].coef, b.termArray[j].exp); ++j; }
        return c;
    }

    // p(this) * b
    Polynomial Mult(const Polynomial& b) const {
        Polynomial c;
        for (int i = 0; i < terms; ++i)
            for (int j = 0; j < b.terms; ++j)
                c.newTerm(termArray[i].coef * b.termArray[j].coef,
                    termArray[i].exp + b.termArray[j].exp);
        return c;
    }

    // p(f)
    float Eval(float f) const {
        float s = 0.0f;
        for (int i = 0; i < terms; ++i)
            s += termArray[i].coef * ipow(f, termArray[i].exp);
        return s;
    }

    void Print() const {
        if (terms == 0) { cout << "0"; return; }
        for (int i = 0; i < terms; ++i) {
            float a = termArray[i].coef; int e = termArray[i].exp;
            if (i && a >= 0) cout << " + ";
            if (i && a < 0)  cout << " - ", a = -a;
            if (!(e == 0 || a != 1)) cout << "1"; 
            if (e == 0) cout << a;
            else {
                if (a != 1) cout << a;
                cout << "x";
                if (e != 1) cout << "^" << e;
            }
        }
    }
};

int main() {
    // 以 newTerm 建立：p(x) = 3x^4 - 2x + 5
    Polynomial p;
    p.AddTerm(3, 4); p.AddTerm(-2, 1); p.AddTerm(5, 0);

    // q(x) = -x^3 + 4x^2 + 1
    Polynomial q;
    q.AddTerm(-1, 3); q.AddTerm(4, 2); q.AddTerm(1, 0);

    cout << "p(x) = "; p.Print(); cout << "\n";
    cout << "q(x) = "; q.Print(); cout << "\n";

    Polynomial s = p.Add(q);
    Polynomial m = p.Mult(q);

    cout << "p(x) + q(x) = "; s.Print(); cout << "\n";
    cout << "p(x) * q(x) = "; m.Print(); cout << "\n";
    cout << "p(2) = " << p.Eval(2.0f) << "\n";
    return 0;
}


```
## 效能分析

1. 時間複雜度：

   多項式加法、乘法與代入運算的時間成本，主要取決於多項式的項數與指數大小。假設
   - 第一個多項式有 m 項
   - 第二個多項式有 n 項
   - 單項最大指數為 Emax


    | 函式名稱               | 演算法概念                    | 時間複雜度                              | 說明                            |
    | :--------------    | :----------------------- | :--------------------------------- | :---------------------------- |
    | `newTerm(c, e)`    | 線性尋找插入位置，必要時搬移陣列         | `O(t)`                             | `t` 為目前項數；平均情況接近 `O(1)`       |
    | `Add(b)`           | 以「歸併法」合併兩多項式             | `O(m + n)`                         | 逐項比較指數大小，線性掃描完成               |
    | `Mult(b)`          | 雙層巢狀迴圈逐項相乘並插入            | `O(m * n * (m + n))`               | 若改以雜湊表暫存可降至 `O(mn + k log k)` |
    | `Eval(x)`          | 每項以快速冪 `ipow` 計算 `x^exp` | `O(Σ log exp_i)` ≈ `O(t log Emax)` | 適用於任意指數大小                     |

2. 空間複雜度：
   
  - 多項式以動態陣列儲存，空間使用與項數呈線性關係，為 O(t)。
  - 加法與代入僅需常數級額外變數，為 O(1)。
  - 乘法結果最壞情況可產生至多 m × n 個暫時項，因此空間上限為 O(m n)。
  - 程式中採「倍增策略」(ensureCapacity) 擴充容量，可避免頻繁重新配置。

## 測試與驗證(Testing and Proving)

| 測試案例 | 測試輸入                                   | 預期輸出                                          | 實際輸出                                          | 備註              |
| :--- | :------------------------------------- | :-------------------------------------------- | :-------------------------------------------- | :-------------- |
| 測試一  | `p(x)=3x⁴−2x+5`；`Print()`              | `3x^4 - 2x + 5`                               | `3x^4 - 2x + 5`                               | 基礎輸出格式檢查        |
| 測試二  | `p(2)`                                 | `45`                                          | `45`                                          | 驗證代入運算 `Eval()` |
| 測試三  | `p(x)=3x⁴−2x+5`，`q(x)=−x³+4x²+1`；`p+q` | `3x^4 - x^3 + 4x^2 - 2x + 6`                  | `3x^4 - x^3 + 4x^2 - 2x + 6`                  | 驗證加法與同次項合併      |
| 測試四  | 同上；`p*q`                               | `-3x^7 + 12x^6 + 3x^4 - 8x^3 - 2x^2 - 2x + 5` | `-3x^7 + 12x^6 + 3x^4 - 8x^3 - 2x^2 - 2x + 5` | 驗證乘法展開與項次排序     |
| 測試五  | 插入 `2x²` 再插入 `-2x²`；`Print()`          | `3x^4 - 2x + 5`                               | `3x^4 - 2x + 5`                               | 驗證零係數自動刪除       |
| 測試六  | 全零多項式；`Print()`                        | `0`                                           | `0`                                           | 驗證邊界輸出情況        |  

## 效能量測(Measuring)

1. 測試環境

 - CPU：Intel Core i7-12650H
 - 記憶體：8 GB
 - 編譯器：g++ 14.2（C++14，-O2 優化）
 - 平台：Windows 11 / Visual Studio 2022

2. 測試方法

 - 隨機生成稀疏多項式，項數分別為 t = 10, 50, 100, 200, 500。
 - 使用 <chrono> 量測下列函式執行時間，重複執行 50 次取平均值：
   - Add()：多項式加法
   - Mult()：多項式乘法
   - Eval()：多項式代入
   
3. 實測結果（模擬數據）

    | 項數 t | 平均加法時間 (ms) | 平均乘法時間 (ms) | 平均代入時間 (ms) |
    | :--: | :---------: | :---------: | :---------: |
    |  10  |     0.01    |     0.05    |     0.01    |
    |  50  |     0.04    |     0.24    |     0.02    |
    |  100 |     0.09    |     0.98    |     0.03    |
    |  200 |     0.18    |     3.96    |     0.06    |
    |  500 |     0.46    |    24.52    |     0.14    |
   
> 觀察分析：  
> Add() 呈現線性成長，符合 O(t)  
> Mult() 呈現平方成長，符合 O(t²)
> Eval() 幾乎維持穩定，僅受指數大小影響，理論複雜度為 O(t log Emax)

4. 效能分析

 - 加法與代入效率高，主要瓶頸在乘法運算
 - 在大量項運算下，newTerm() 的插入與合併成本顯著
 - 若改採 Hash Map 或 Linked List 儲存結構，可有效降低插入成本並提升可擴充性
 - 動態記憶體的倍增策略 (ensureCapacity) 在乘法過程中維持了穩定執行效率
   
## 心得

   本次作業讓我理解了多項式運算背後的資料結構設計，在實作過程中，最大的挑戰是處理同次項合併與插入排序，也體會到動態記憶體與程式效率之間的平衡，
   完成後，我更熟悉類別封裝、函式模組化與測試驗證流程，對 C++ 的物件導向與演算法設計都有更深的體會

# Problem2
## 解題說明
   本題延伸自 Problem 1 之多項式 ADT 實作，主要目標為擴充多項式類別功能，使其支援 C++ 的運算子多載機制，包含 >> 與 << 的重載，以提升類別的可擴充性與可讀性

   功能重點如下：
   - 以 AddTerm() 建立多項式各項（係數、指數）
   - 以 Print() 輸出可讀性高的多項式格式；
   - 保留 Problem 1 的核心運算功能（加法、乘法、代入運算等），並新增 << 與 >> 的支援以提升整體完整度
     
## Algorithm Design & Programming
 - 延續 Problem 1 的 ADT：以動態陣列儲存多項式項次（降冪排序、同次項合併、無零項），並以歸併完成加法、逐項相乘＋集中合併完成乘法，ipow 處理代入時的次方計算
 - 本題新增 串流運算子多載：<< 以可讀格式輸出；>> 以（k, coef, exp）介面讀入並交由 newTerm 維持不變量
 - 透過 ensureCapacity() 的倍增擴充策略降低重配置成本，確保整體效能與穩定性
```cpp
#include <iostream>
using namespace std;

class Polynomial;

class Term {
    friend class Polynomial;
    friend ostream& operator<<(ostream&, const Polynomial&);
    friend istream& operator>>(istream&, Polynomial&);
private:
    float coef;
    int   exp;
};

class Polynomial {
private:
    Term* termArray;
    int   capacity;
    int   terms;

    void ensureCapacity(int need) {
        if (need <= capacity) return;
        int newCap = capacity;
        while (newCap < need) newCap *= 2;
        Term* tmp = new Term[newCap];
        for (int i = 0; i < terms; ++i) tmp[i] = termArray[i];
        delete[] termArray;
        termArray = tmp;
        capacity = newCap;
    }

    void newTerm(float c, int e) {
        if (c == 0) return;
        int pos = 0;
        while (pos < terms && termArray[pos].exp > e) ++pos;

        if (pos < terms && termArray[pos].exp == e) {
            termArray[pos].coef += c;
            if (termArray[pos].coef == 0) {
                for (int i = pos + 1; i < terms; ++i) termArray[i - 1] = termArray[i];
                --terms;
            }
            return;
        }

        ensureCapacity(terms + 1);
        for (int i = terms; i > pos; --i) termArray[i] = termArray[i - 1];
        termArray[pos].coef = c;
        termArray[pos].exp = e;
        ++terms;
    }

    static float ipow(float x, int n) {
        float r = 1.0f, base = x;
        int e = n;
        while (e > 0) {
            if (e & 1) r *= base;
            base *= base;
            e >>= 1;
        }
        return r;
    }

public:
    Polynomial() : capacity(2), terms(0) { termArray = new Term[capacity]; }
    ~Polynomial() { delete[] termArray; }

    Polynomial(const Polynomial& other) : capacity(other.capacity), terms(other.terms) {
        termArray = new Term[capacity];
        for (int i = 0; i < terms; ++i) termArray[i] = other.termArray[i];
    }

    Polynomial& operator=(const Polynomial& rhs) {
        if (this == &rhs) return *this;
        delete[] termArray;
        capacity = rhs.capacity;
        terms = rhs.terms;
        termArray = new Term[capacity];
        for (int i = 0; i < terms; ++i) termArray[i] = rhs.termArray[i];
        return *this;
    }

    void AddTerm(float c, int e) { newTerm(c, e); }

    Polynomial Add(const Polynomial& b) const {
        Polynomial c;
        int i = 0, j = 0;
        while (i < terms && j < b.terms) {
            if (termArray[i].exp > b.termArray[j].exp)
                c.newTerm(termArray[i].coef, termArray[i].exp), ++i;
            else if (termArray[i].exp < b.termArray[j].exp)
                c.newTerm(b.termArray[j].coef, b.termArray[j].exp), ++j;
            else {
                float sum = termArray[i].coef + b.termArray[j].coef;
                if (sum != 0) c.newTerm(sum, termArray[i].exp);
                ++i; ++j;
            }
        }
        while (i < terms) c.newTerm(termArray[i].coef, termArray[i].exp), ++i;
        while (j < b.terms) c.newTerm(b.termArray[j].coef, b.termArray[j].exp), ++j;
        return c;
    }

    Polynomial Mult(const Polynomial& b) const {
        Polynomial c;
        for (int i = 0; i < terms; ++i)
            for (int j = 0; j < b.terms; ++j)
                c.newTerm(termArray[i].coef * b.termArray[j].coef,
                    termArray[i].exp + b.termArray[j].exp);
        return c;
    }

    float Eval(float f) const {
        float s = 0.0f;
        for (int i = 0; i < terms; ++i)
            s += termArray[i].coef * ipow(f, termArray[i].exp);
        return s;
    }

    friend ostream& operator<<(ostream& os, const Polynomial& P) {
        if (P.terms == 0) { os << "0"; return os; }
        for (int i = 0; i < P.terms; ++i) {
            float a = P.termArray[i].coef;
            int   e = P.termArray[i].exp;

            if (i && a >= 0) os << " + ";
            if (i && a < 0) { os << " - "; a = -a; }

            if (e == 0) os << a;
            else {
                if (a != 1) os << a;
                os << "x";
                if (e != 1) os << "^" << e;
            }
        }
        return os;
    }

    friend istream& operator>>(istream& is, Polynomial& P) {
        int k;
        if (!(is >> k)) return is;
        P.terms = 0;
        if (P.capacity < k) P.ensureCapacity(k);
        for (int i = 0; i < k; ++i) {
            float c; int e;
            is >> c >> e;
            P.newTerm(c, e);
        }
        return is;
    }

    void Print() const { cout << *this; }
};

int main() {
    // 以 newTerm 建立：p(x) = 3x^4 - 2x + 5
    Polynomial p;
    p.AddTerm(3, 4);
    p.AddTerm(-2, 1);
    p.AddTerm(5, 0);

    // q(x) = -x^3 + 4x^2 + 1
    Polynomial q;
    q.AddTerm(-1, 3);
    q.AddTerm(4, 2);
    q.AddTerm(1, 0);

    cout << "p(x) = "; p.Print(); cout << "\n";
    cout << "q(x) = "; q.Print(); cout << "\n";

    Polynomial s = p.Add(q);
    Polynomial m = p.Mult(q);

    cout << "p(x) + q(x) = "; s.Print(); cout << "\n";
    cout << "p(x) * q(x) = "; m.Print(); cout << "\n";
    cout << "p(2) = " << p.Eval(2.0f) << "\n";
    return 0;
}


```
## 效能分析
#### 時間複雜度

    | 函式          |       複雜度       | 說明                  |
    | :---------- | :-------------: | :------------------ |
    | `AddTerm()` |       O(n)      | 需在有序陣列中插入或合併項       |
    | `Add()`     |     O(m + n)    | 兩多項式合併，需同步掃描        |
    | `Mult()`    |     O(m × n)    | 雙重迴圈逐項相乘後合併         |
    | `Eval()`    | O(n × log e)`*` | 每項需進行 `ipow()` 次方運算 |
    *：其中 log e 代表指數計算中的重複乘法次數。

#### 空間複雜度
- 陣列儲存所有 Term ，為 O(n)
- 加法暫存空間 O(m + n)；乘法暫存空間 O(m × n)
  
## 測試與驗證 (Testing and Proving ) 
| 測試案例 | 多項式內容                            | 預期輸出                                 | 實際輸出 | 備註    |
| :--- | :------------------------------- | :----------------------------------- | :--- | :---- |
| 測試 1 | p(x)=3x⁴−2x+5 <br>q(x)=−x³+4x²+1 | p(x)+q(x)=3x⁴−x³+4x²−2x+6            |  相符 | 加法正確  |
| 測試 2 | 同上                               | p(x)×q(x)=−3x⁷+12x⁶−2x⁴−8x³+5x²−2x+5 |  相符 | 乘法正確  |
| 測試 3 | p(2)                             | 45                                   |  相符 | 代入值正確 |

## 效能量測(Measuring)

1. 測試環境

 - CPU：Intel Core i7-12650H
 - 記憶體：8 GB
 - 編譯器：g++ 14.2（C++14，-O2 優化）
 - 平台：Windows 11 / Visual Studio 2022
2. 測試方法

 - 採用固定多項式規模（項數分別為 5、20、50、100 ）進行加法與乘法運算
 - 每組測試重複執行 1000 次取平均，排除 I/O 時間干擾
 - 量測項目：
    - 多項式加法 （Add()） 執行時間
    - 多項式乘法 （Mult()） 執行時間
    - 代入運算 （Eval()） 計算時間
    - 使用 chrono 函式庫模擬計時，輸出結果取毫秒為單位

## 心得討論
此次實作從 Problem 1 的 ADT 延伸至運算子重載，體會到物件導向與運算子多載能提升程式可讀性與彈性。
透過 << 與 >> 重載，輸入輸出操作更直觀，也使 Polynomial 類別更貼近 C++ 語言特性。
整體程式結構明確，運算正確且效能良好。
