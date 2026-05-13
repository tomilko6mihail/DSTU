#include <iostream>
using namespace std;

int readInt(const string& prompt) {
    string line;
    while (true) {
        cout << prompt;
        getline(cin, line);
        if (line.empty()) { cout << "Ошибка:  введите число\n"; continue; }
        size_t pos;
        try {
            int value = stoi(line, &pos);
            if (pos == line.size()) return value;
        } catch(...) {}
        cout << "Ошибка: введите целое число\n";
    }
}

int readPositiveInt(const string& prompt) {
    while (true) {
        int n = readInt(prompt);
        if (n > 0) return n;
        cout << "Ошибка: число > 0\n";
    }
}

struct Node {
    int data;
    Node* next;
};

struct TStack {
    Node* Top;
};


void CreateStack(TStack& S) {
    S.Top = nullptr;
}

void Push(TStack& S, int D) {
    Node* p = new Node;
    p->data = D;
    p->next = S.Top;
    S.Top = p;
}

int Pop(TStack& S) {
    if (S.Top == nullptr) return 0;
    Node* p = S.Top;
    int val = p->data;
    S.Top = p->next;
    delete p;
    return val;
}

void PrintStack(TStack& S) {
    Node* cur = S.Top;
    while (cur) {
        cout << cur->data;
        if (cur->next) cout << " -> ";
        cur = cur->next;
    }
    cout << endl;
}

void RemoveBetween(TStack& S) {
    if (S.Top == nullptr || S.Top->next == nullptr) return;

    int len = 0;
    Node* cur = S.Top;
    while (cur) { len++; cur = cur->next; }
    
    int* arr = new int[len];
    cur = S.Top;
    for (int i = 0; i < len; i++) {
        arr[i] = cur->data;
        cur = cur->next;
    }
    
    int minIdx = 0, maxIdx = 0;
    for (int i = 1; i < len; i++) {
        if (arr[i] < arr[minIdx]) minIdx = i;
        if (arr[i] > arr[maxIdx]) maxIdx = i;
    }
    
    int left = min(minIdx, maxIdx);
    int right = max(minIdx, maxIdx);
    
    if (right - left > 1) {
        TStack newStack;
        CreateStack(newStack);
        
        for (int i = len - 1; i >= 0; i--) {
            if (i > left && i < right) continue;
            Push(newStack, arr[i]);
        }
        
        while (S.Top) Pop(S);

        S.Top = newStack.Top;
    }
    
    delete[] arr;
}

void LinkedPart() {
    cout << "\n===== ЦЕПНОЕ ПРЕДСТАВЛЕНИЕ =====\n";
    
    TStack S;
    CreateStack(S);
    
    int N = readPositiveInt("Введите N: ");
    cout << "Введите " << N << " чисел:\n";
    for (int i = 0; i < N; i++) {
        int x = readInt("  > ");
        Push(S, x);
    }
    
    cout << "Стек (вершина -> основание): ";
    PrintStack(S);
    cout << "Адрес вершины стека: " << S.Top << endl;
    
    RemoveBetween(S);
    
    cout << "Стек после удаления: ";
    PrintStack(S);
    
    while (S.Top) Pop(S);
}

struct AStack {
    int* data;
    int top;
    int size;
};

void CreateArrayStack(AStack& S, int initialSize = 10) {
    S.data = new int[initialSize];
    S.top = -1;
    S.size = initialSize;
}

void PushArray(AStack& S, int D) {
    if (S.top + 1 >= S.size) {
        int* newData = new int[S.size * 2];
        for (int i = 0; i <= S.top; i++) newData[i] = S.data[i];
        delete[] S.data;
        S.data = newData;
        S.size *= 2;
    }
    S.data[++S.top] = D;
}

int PopArray(AStack& S) {
    if (S.top == -1) return 0;
    return S.data[S.top--];
}

void PrintArrayStack(AStack& S) {
    for (int i = S.top; i >= 0; i--) {
        cout << S.data[i];
        if (i > 0) cout << " -> ";
    }
    cout << endl;
}

void RemoveBetweenArray(AStack& S) {
    if (S.top < 1) return;
    
    int len = S.top + 1;
    int minIdx = 0, maxIdx = 0;
    
    for (int i = 1; i < len; i++) {
        if (S.data[i] < S.data[minIdx]) minIdx = i;
        if (S.data[i] > S.data[maxIdx]) maxIdx = i;
    }
    
    int left = min(minIdx, maxIdx);
    int right = max(minIdx, maxIdx);
    
    if (right - left > 1) {
        int removeCount = right - left - 1;
        for (int i = right; i <= S.top; i++) {
            S.data[i - removeCount] = S.data[i];
        }
        S.top -= removeCount;
    }
}

void ArrayPart() {
    cout << "\n===== СПЛОШНОЕ ПРЕДСТАВЛЕНИЕ =====\n";
    
    AStack S;
    CreateArrayStack(S);
    
    int N = readPositiveInt("Введите N: ");
    cout << "Введите " << N << " чисел:\n";
    for (int i = 0; i < N; i++) {
        int x = readInt("  > ");
        PushArray(S, x);
    }
    
    cout << "Стек (вершина -> основание): ";
    PrintArrayStack(S);
    cout << "Адрес вершины стека: " << &S.data[S.top] << endl;
    
    RemoveBetweenArray(S);
    
    cout << "Стек после удаления: ";
    PrintArrayStack(S);
    
    delete[] S.data;
}

int main() {
    LinkedPart();
    ArrayPart();
    return 0;
}
