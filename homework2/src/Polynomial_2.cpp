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

