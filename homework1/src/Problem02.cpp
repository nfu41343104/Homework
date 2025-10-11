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
