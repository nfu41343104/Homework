#include <iostream>
#include <cstdlib>

using namespace std;

// 抽象類別 MinPQ
template <class T>
class MinPQ {
public:
    virtual ~MinPQ() {}
    virtual bool IsEmpty() const = 0;
    virtual const T& Top() const = 0;
    virtual void Push(const T& x) = 0;
    virtual void Pop() = 0;
};

// MinHeap 實作
template <class T>
class MinHeap : public MinPQ<T> {
private:
    T* heap;
    int capacity;
    int heapSize;

    void Resize() {
        capacity *= 2;
        T* newHeap = new T[capacity];
        for (int i = 1; i <= heapSize; i++) {
            newHeap[i] = heap[i];
        }
        delete[] heap;
        heap = newHeap;
    }

    void BubbleUp(int index) {
        while (index > 1) {
            int parent = index / 2;
            if (heap[parent] <= heap[index]) break;

            T temp = heap[parent];
            heap[parent] = heap[index];
            heap[index] = temp;

            index = parent;
        }
    }

    void BubbleDown(int index) {
        while (2 * index <= heapSize) {
            int child = 2 * index;

            if (child + 1 <= heapSize && heap[child + 1] < heap[child]) {
                child++;
            }

            if (heap[index] <= heap[child]) break;

            T temp = heap[index];
            heap[index] = heap[child];
            heap[child] = temp;

            index = child;
        }
    }

public:
    MinHeap(int initialCapacity = 10) {
        capacity = initialCapacity + 1; // index 從 1 開始
        heapSize = 0;
        heap = new T[capacity];
    }

    ~MinHeap() {
        delete[] heap;
    }

    bool IsEmpty() const override {
        return heapSize == 0;
    }

    const T& Top() const override {
        if (IsEmpty()) {
            cerr << "Heap is empty.\n";
            exit(1);
        }
        return heap[1];
    }

    void Push(const T& x) override {
        if (heapSize + 1 == capacity) {
            Resize();
        }

        heap[++heapSize] = x;
        BubbleUp(heapSize);
    }

    void Pop() override {
        if (IsEmpty()) {
            cerr << "Heap is empty.\n";
            exit(1);
        }

        heap[1] = heap[heapSize--];

        if (!IsEmpty()) {
            BubbleDown(1);
        }
    }
};


int main() {
    MinHeap<int> h;

    h.Push(20);
    h.Push(5);
    h.Push(15);
    h.Push(2);
    h.Push(8);

    cout << "Current Min: " << h.Top() << endl;

    h.Pop();
    cout << "After Pop Min: " << h.Top() << endl;

    h.Pop();
    cout << "After Pop Min: " << h.Top() << endl;

    return 0;
}
