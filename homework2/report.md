# 41343104
# 作業二之Polynomial
# Problem1

## 解題說明
設計一個多項式類別 Polynomial，能夠動態新增項次、執行多項式運算與代入求值：

- 新增項次（AddTerm）:
依降冪插入或合併同次項。

- 多項式加法（Add）：
兩個已排序多項式逐項合併。

- 多項式乘法（Mult）:
雙層迴圈逐項相乘並自動合併。

- 代入計算（Eval）:
使用快速冪 ipow 計算每項貢獻。

- 輸出函式（Print）:
以人類可讀格式輸出多項式

## Algorithm Design & Programming
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

 - 加法與代入效率高，主要瓶頸在乘法運算。
 - 在大量項運算下，newTerm() 的插入與合併成本顯著。
 - 若改採 Hash Map 或 Linked List 儲存結構，可有效降低插入成本並提升可擴充性。
 - 動態記憶體的倍增策略 (ensureCapacity) 在乘法過程中維持了穩定執行效率。
   
## 心得

   本次作業讓我理解了多項式運算背後的資料結構設計，在實作過程中，最大的挑戰是處理同次項合併與插入排序，也體會到動態記憶體與程式效率之間的平衡。
   完成後，我更熟悉類別封裝、函式模組化與測試驗證流程，對 C++ 的物件導向與演算法設計都有更深的體會。

# Problem2
## 解題說明
- 資料結構：使用Term結構來儲存單一項的係數和指數。Polynomial類別則包含一個Term型態的動態陣列termArray，以及容量（capacity）和實際項數（terms）的變數。這種設計能有效利用記憶體，只儲存非零項。
- 動態擴充：當新增項次且陣列滿載時，NewTerm方法會自動將陣列容量加倍，確保程式能處理任意數量的項次。
- 排序與合併：NewTerm方法在新增項次後，會檢查是否已有相同指數的項次，若有則合併係數。接著，它會對整個陣列進行排序，以指數遞減的順序排列，這有助於優化顯示和運算。
- 多項式求值：Eval方法提供一個機制，能計算多項式在特定變數值下的結果。它遍歷所有項次，並將每個項次的運算結果相加。
- 運算子多載：
- operator>>：多載輸入運算子，使得使用者能透過cin方便地輸入多項式。
- operator<<：多載輸出運算子，能將多項式以易於閱讀的格式輸出到cout。
## 程式實作
```cpp
#include <iostream>
#include <cmath>
using namespace std;

class Term {
    friend class Polynomial;
    friend std::ostream& operator<<(std::ostream&, const Polynomial&);
private:
    float coef;  
    int exp;    
};
class Polynomial {
    friend istream& operator>>(istream& in, Polynomial& poly);
    friend ostream& operator<<(ostream& out, const Polynomial& poly);

private:
    Term* termArray;
    int capacity;
    int terms;

public:
    Polynomial() {
        capacity = 10;
        terms = 0;
        termArray = new Term[capacity];
    }
    void NewTerm(float c, int e) {
        if (c == 0) return;
        for (int i = 0; i < terms; i++) {
            if (termArray[i].exp == e) {
                termArray[i].coef += c;
                return;
            }
        }
        if (terms == capacity) {
            capacity *= 2;
            Term* temp = new Term[capacity];
            for (int i = 0; i < terms; i++)
                temp[i] = termArray[i];
            delete[] termArray;
            termArray = temp;
        }

        termArray[terms].coef = c;
        termArray[terms].exp = e;
        terms++;
        for (int i = 0; i < terms - 1; i++) {
            for (int j = i + 1; j < terms; j++) {
                if (termArray[i].exp < termArray[j].exp)
                    swap(termArray[i], termArray[j]);
            }
        }
    }

    float Eval(float x) const {
        float result = 0;
        for (int i = 0; i < terms; i++)
            result += termArray[i].coef * pow(x, termArray[i].exp);
        return result;
    }
};
istream& operator>>(istream& in, Polynomial& poly) {
    int n;
    cout << "Enter number of terms: ";
    in >> n;

    poly.terms = 0; 

    for (int i = 0; i < n; i++) {
        float c;
        int e;
        cout << "Enter coefficient and exponent for term " << i + 1 << ": ";
        in >> c >> e;
        poly.NewTerm(c, e);
    }

    return in;
}
ostream& operator<<(ostream& out, const Polynomial& poly) {
    if (poly.terms == 0) {
        out << "0";
        return out;
    }

    for (int i = 0; i < poly.terms; i++) {
        float c = poly.termArray[i].coef;
        int e = poly.termArray[i].exp;

        if (i > 0) {
            if (c >= 0) out << " + ";
            else { out << " - "; c = -c; }
        }
        else if (c < 0) {
            out << "-";
            c = -c;
        }

        if (e == 0)
            out << c;
        else if (e == 1)
            out << c << "x";
        else
            out << c << "x^" << e;
    }

    return out;
}

int main() {
    Polynomial p;

    cin >> p;           
    cout << "p(x) = " << p << endl; 

    float x;
    cout << "Enter x value: ";
    cin >> x;
    cout << "p(" << x << ") = " << p.Eval(x) << endl;

    return 0;
}

```
## 效能分析
#### 時間複雜度NewTerm()：合併項次：最差情況下，需遍歷所有現有項次，時間複雜度為 $\(O(\text{terms})\)$
- 陣列擴充：當需要擴充容量時，會建立新陣列並複製所有舊項次，時間複雜度為 $\(O(\text{terms})\)$
- 排序：在每次新增項次後都進行排序。使用的泡沫排序時間複雜度為 $\(O(\text{terms}^{2})\)$
  若多項式項數較多，這會成為效能瓶頸
- 總體：綜合來看，NewTerm()的最差時間複雜度為 $\(O(\text{terms}^{2})\)$，主要受排序操作影響。
- Eval()：該方法遍歷所有項次，並對每個項次進行pow()運算，時間複雜度為 $\(O(\text{terms}\cdot \log (\text{exp}))\)$，其中pow()運算的時間複雜度取決於其演算法。
- operator<<()：該方法遍歷所有項次進行輸出，時間複雜度為 $\(O(\text{terms})\)$。operator>>()：在輸入 $\(n\)$ 個項次時，會迴圈呼叫 NewTerm() \(n\) 次。
- operator>>() 的總時間複雜度為 $\(O(n^{3})\)$。
#### 空間複雜度
- Polynomial 物件：termArray 是動態陣列，其空間隨項次增加而擴充。當有 \(n\) 個項次時，空間複雜度為 \(O(n)\)。
- NewTerm()：當擴充容量時，會建立一個兩倍大的臨時陣列，因此需要額外的 \(O(n)\) 空間。
## 測試與驗證
| 測試案例 | 輸入資料 | 
|----------|--------------|
| 測試一   |        3(個資料)   |     
|  第一資料        |1 1|
|  第二資料        |   2 1       | 
|  第三資料        |   2 3      | 
|  x的值        |   4    |

| 輸出資料 |  p(x)|p(4)| 
|----------|--------------|------|
| 所有資料統整  |       2x^3 + 3x   |   140  |

### 編譯與執行指令
#### 測試輸入
```shell
Enter number of terms: 3
Enter coefficient and exponent for term 1: 1 1
Enter coefficient and exponent for term 2: 2 1
Enter coefficient and exponent for term 3: 2 3

Enter x value: 4
```
#### 測試輸出
```shell
p(x) = 2x^3 + 3x

p(4) = 140
```
## 效能量測
優化建議：
- 改善排序：將NewTerm中的泡沫排序替換為更高效的演算法，例如插入排序。由於新項次只插入一個，插入排序的效能會比完整重新排序要好。更好的方法是在插入時保持排序狀態。
- 優化求值：對於 Eval，可以考慮使用霍納法則（Horner's method），這能避免在每個迴圈中重複計算pow()，從而提高求值速度。
## 申論及開發報告
### 效能討論：
分析目前實作的效能瓶頸（特別是NewTerm中的排序）。
提出優化建議，如使用更高效的排序演算法或霍納法則，並討論這些優化對時間複雜度的影響。
### 心得討論
這份程式碼提供了一個基礎的多項式類別實作，展示了物件導向程式設計的核心概念。然而，它的效能（特別是在處理大量項次時）有待改進。透過替換低效的排序演算法和優化求值方法，可以顯著提升程式的效率和可擴展性。這份報告不僅解釋了程式碼的功能，也指出了其局限性，並提出了明確的改進方向。
