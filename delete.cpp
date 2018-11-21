/**
 *
 *	Author : Jangtaejin < jtjisgod@gmail.com >
 *	Date : 2018-11-21 (Wednesday)
 *
 */

#include <iostream>
#include <stack>

using namespace std;

class Node {
    public:
        int n;
        int* K;     // Key
        Node** P;    // Pointer
};

void insertBT(Node*& T, int m, int newKey);
void inorderBT(Node*& T, int m);

Node* getNode(int m) {
    // m += 1; // 오버플로우시 잠시 넣어놓기위해 1이 더 커야한다.
    Node* node = new Node;
    node -> n = 0;
    node -> K = new int[m];
    node -> P = new Node*[m+1];
    return node;
}

int main(int argc, char** argv) {
    int inp[] = {30, 20, 62, 110, 140, 15, 65, 136, 150, 120, 40, 132, 19, 128, 138, 100, 16, 145, 70, 42, 69, 43, 26, 60, 130, 50, 18, 7, 36, 58, 22, 41, 59, 57, 54, 33, 75, 124, 122, 123};
    cout << "[ M=3, B-Tree ]" << endl;
    Node *T = NULL;
    for(int i=0;i<40;i++) {
        insertBT(T, 3, inp[i]);
        inorderBT(T, 3);
        cout << endl;
    }
    cout << endl;
    cout << "[ M=4, B-Tree ]" << endl;
    T = NULL;
    for(int i=0;i<40;i++) {
        insertBT(T, 4, inp[i]);
        inorderBT(T, 4);
        cout << endl;
    }
    return 0;
}

// 출력
void inorderBT(Node*& T, int m) {
    if(T == NULL) {
        return;
    }
    int i;
    for(i=0;i<T->n;i++) {
        inorderBT(T -> P[i], m);
        cout << T -> K[i] << " ";
    }
    inorderBT(T -> P[i], m);
}

// Btree에서 Key를 제거한다.
void deleteBT(Node*& T, int m, int oldKey) {
    
}

// Btree에 Key를 추가한다.
void insertBT(Node*& T, int m, int newKey) {

    // 디버깅을 위한 출력
    // cout << "\nNewkey : " << newKey << endl;

    // 만약 최상단이 NULL이라면
    if(T == NULL) {
        T = getNode(m);
        T -> K[0] = newKey;
        T -> n = 1;
        return;
    }

    // 여기저기 돌아다니는 x = T
    Node* x = T;
    Node* p = NULL;
    int i;

    // 탐험 Stack
    stack<Node*> s;

    do {

        i = 0;

        // 적당한 부분을 찾는다.
        while(i < x -> n && newKey > x -> K[i])     { i += 1; }

        // 같은거 나오면 안되니까 오류 -> 함수 종료
        if(i < x -> n && newKey == x -> K[i])      {return; }

        // 지나온 길 등록
        s.push(x);
        // cout << "Path : " << i << endl;
        x = x -> P[i];

    } while(x != NULL); // x는 해당 위치의 Node를 가리키는 포인터이며
                        // null인경우 종료

    // 반복문을 종료하기 위한 Flag
    bool finished = false;

    // 삽입할 위치
    x = s.top(); s.pop();

    do {

        // 적당한 위치에 키를 넣어준다.
        i = 0;
        while(i < x -> n && newKey > x -> K[i]) {
            i += 1;
        }

        for(int j = x -> n; j > i; j--) {
            x -> K[j] = x -> K[j-1];
            x -> P[j+1] = x -> P[j];
            x -> P[j] = x -> P[j-1];
        }

        if(p) {
            x -> P[i] = p -> P[0];
            x -> K[i] = p -> K[0];
            x -> P[i+1] = p -> P[1];
            x -> n += 1;
        } else {
            x -> K[i] = newKey;
            x -> n += 1;
        }

        // 오버플로우가 아니므로 반복문 종료
        if(x -> n < m) {
            break;
        }

        // cout << "Overflow" << endl;

        // x를 잠시 임시 노드로 보내고,
        Node* tempNode = x;
        x = getNode(m);

        // 중간 선택
        int middle = (int)((tempNode->n)/2);

        // 왼쪽은 x에 재삽입
        for(i=0;i<middle;i++) {
            x -> K[i] = tempNode -> K[i];
            x -> P[i] = tempNode -> P[i];
            x -> n ++;
        }
        x -> P[i] = tempNode -> P[i];

        Node* y = NULL;
        if(middle+1 < tempNode -> n) {
            y = getNode(m);
            // 오른쪽은 y에 재삽입
            for(i=middle+1;i<tempNode->n;i++) {
                int k = i-middle-1;
                y -> K[k] = tempNode -> K[i];
                y -> P[k] = tempNode -> P[i];
                y -> n ++;
            }
            y -> P[i-middle-1] = tempNode -> P[i];
        }

        // 새로운키가 바뀌었다!
        newKey = tempNode -> K[middle];
        p = getNode(m);
        p -> K[0] = newKey;
        p -> P[0] = x;
        p -> P[1] = y;
        p -> n = 1;

        // 분배를 다 해주고 쓸모가 없어서 Free
        delete tempNode;

        if(s.size() == 0) {
            T = p;
            finished = true;
        } else {
            x = s.top();
            s.pop();
        }
    } while(!finished);
}
