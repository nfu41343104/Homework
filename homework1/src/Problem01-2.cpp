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
