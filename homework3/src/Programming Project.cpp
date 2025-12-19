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

