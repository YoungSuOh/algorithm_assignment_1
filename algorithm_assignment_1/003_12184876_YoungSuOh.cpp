#include<iostream>
#include<string>
#include<vector>
#include<cmath>
#define endl "\n"
using namespace std;

class Heap // Heap class 생성
{
private:
    vector<int>v; // vector를 쓰므로써 
    int heapSize; // heapSize 멤버변수로 설정
public:
    int vacant = 0; // vacant값 초기에 0으로 설정 -> total time을 count하기 위한 변수
    Heap()
    {
        v.push_back(-1); // 부모와 자식간의 관계를 두배로 유지하기 위해 index 0번에 임의의 값을 집어넣고 사용하지 않는다.
        this->heapSize = 0; // 초기에 heapSize는 0으로 설정
    }
    ~Heap() {} // 소멸자 함수
    void constructHeap(int root_idx) // heap 만드는 함수
    {
        if (getCurrentHeight(root_idx) != 0) { // leaf가 아닐경우에 해당 조건문 실행
            constructHeap(2 * root_idx); // root_idx가 root인 left subtree로 이동
            constructHeap(2 * root_idx + 1); // root_idx가 root인 right subtree로 이동
            int k = root_idx;
            downheap(k); // 재귀를 해주므로써 heap의 leaf까지 이동하여 heap의 순서조건에 만족하도록 downheap 진행
        }
        return;
    }
    void upheap(int idx) // 자식과 부모를 비교하면서 heap의 순서조건을 만족시키는 upheap 함수
    {
        if (idx <= 1)
            return;
        else if (v[idx] > v[idx / 2]) // 자식이 더 크면 swap => MAXHEAP
        {
            vacant++; // vacant가 parent쪽으로 변경됐으므로 1추가
            swap(idx, idx / 2);
            upheap(idx / 2); // 재귀 진행
        }
    }
    void downheap(int idx) // 두자식간의 크기를 비교하여 큰 자식을 부모와 비교하는 downheap함수
    {
        int max; // 두 자식 중 크기가 큰 자식의 index 임시 변수 max로 설정
        if (2 * idx + 1 <= heapSize) // 오른쪽 자식이 있을 경우의 조건문 -> 할당된 메모리 공간을 넘을 경우를 방지(segmentFault)
        {
            if (v[2 * idx] > v[2 * idx + 1]) // 오른쪽 자식이 클경우
                max = 2 * idx; // 오른쪽 자식의 index값을 max에 저장
            else // 왼쪽 자식이 클경우
                max = 2 * idx + 1; // 왼쪽 자식의 index값을 max에 저장
            if (v[idx] < v[max]) // 큰 자식의 값이 부모보다 클 경우 swap후 downheap 재귀
            {
                swap(idx, max);
                downheap(max);
            }
            return;
        }
        else if (2 * idx == heapSize) // 왼쪽 자식만 있을경우
        {
            if (v[idx] < v[2 * idx]) // 왼쪽 자식과 비교하여 왼쪽자식이 크면 부모와 swap
            {
                swap(idx, 2 * idx);
            }
            return;
        }
    }
    void downheap_H1(int current) { // fixHeapfast를 진행할 경우 height가 1일때 vacant값을 세기 위한 함수, 하는 역할은 downheap과 동일
        int max;
        if (2 * current + 1 <= heapSize) {
            if (v[2 * current] <= v[2 * current + 1])
                max = 2 * current + 1;
            else
                max = 2 * current;
            if (v[current] <= v[max])
            {
                swap(current, max);
                vacant++;
            }
        }
        else if (2 * current == heapSize) {
            if (v[current] <= v[2 * current]) {
                swap(current, 2 * current);
                vacant++;
            }
        }
        return;
    }
    void fixHeapFast(int current, int height, int key) // accelerate heapsort를 위한 fixHeapSort
    {
        if (height <= 1) { // 높이가 1일경우에는 downheap_H1을 실행한다.
            downheap_H1(current);
            return;
        }
        else {
            int hstop = height / 2; // height/2까지 내려가서 멈춤
            int vacStop = promote(hstop, current); // height/2까지 key와 자식들중 큰자식과 swap하고 promote함수 return값 vacStop에 저장
            int vacParent = vacStop / 2; // vacparent index 설정
            if (v[vacParent] <= v[vacStop]) { // height/2까지 내려온 시점에서 parent와 비교하여 parent보다 현재 위치의 data값이 더 크면
                swap(vacParent, vacStop); // parent와 swap
                vacant++;  // vacant가 이동했으므로 +1 추가
                upheap(vacParent); // heap 구조조건을 만족하기 위해 upheap진행
            }
            else
                fixHeapFast(vacStop, hstop, key); // height가 0또는 1이 될때까지 재귀 진행
        }
        return;
    }
    int promote(int hstop, int current) // height/2까지 이동하면서 자식들 중 큰자식과 key를 swap해주는 함수
    {
        int vacstop;
        if (getCurrentHeight(current) <= hstop) { // 멈춰야 하는 위치 hstop에 도달했을때 
            vacstop = current; // 멈추고 해당 위치를 vacStop
        }
        else if (v[2 * current] <= v[2 * current + 1]) { // 오른쪽 자식이 큰경우
            swap(current, 2 * current + 1); // 오른쪽 자식과 swap
            vacstop = promote(hstop, 2 * current + 1); // 그후 멈춰야하는 위치까지 재귀 진행
            vacant++; // vacant위치 바뀌었으므로 vacant +1
        }
        else { // 왼쪽 자식이 큰 경우
            swap(current, 2 * current); // 왼쪽자식과 swap
            vacstop = promote(hstop, 2 * current); // 그후 멈춰야하는 위치까지 재귀 진행
            vacant++; // vacant위치 바뀌었으므로 vacant +1
        }
        return vacstop;
    }
    void insert(int data) // heap에 data를 순차적으로 넣어주는 함수
    {
        heapSize++; // data를 넣을때마다 heapSize 하나씩 증가
        v.push_back(data);
    }
    bool isEmpty() // heap이 비었는지 아닌지 확인하여 true / false로 반환해주는 함수
    {
        return heapSize == 0;
    }
    void DeleteMAX() // index 1에 있는 값을 없애고 index의 끝에 있는 값 index 1쪽으로 접근하여 accelerate heapsort 해주는 함수
    {
        if (isEmpty()) // heap이 비어있다면 return
            return;
        else
        {
            v[1] = v[heapSize]; // index 1에 있는 값을 없애고 index의 끝에 있는 값 index 1쪽으로 접근
            v.pop_back(); // index 가장 끝 지워줌
            heapSize--; // 기존 index 1에 있던 값 없어졌으므로 heapSize -1
            if (isEmpty()) { return; } // 마지막에 heap에 값이 하나 남고 그 값을 위에서 빼줬을때 case
            else {
                vacant++; // index의 끝에 있는 값 index 1쪽으로 접근했으므로 vacant +1
                fixHeapFast(1, getHeapHeight(), v[1]); // accelerate heapsort 진행
            }
            return;
        }
    }
    void print() // 현재 heap에 있는 값 출력해주는 함수
    {
        if (isEmpty())
            cout << -1 << endl;
        else
        {
            for (unsigned int i = 1; i < v.size(); i++)
            {
                cout << v[i] << " ";
            }
            cout << endl;
        }
    }
    int getHeapHeight() { // ceil(log2(heapSize+1))-1 에서 heapSize를 넣어주므로써 heap의 height는 반환할 수 있다.
        return (ceil(log2(heapSize + 1)) - 1);
    }
    int getCurrentHeight(int idx) { // idx의 height를 구해주는 함수 -> ceil(log2(idx+1))-1 은 idx의 depth를 나타내주고 이것을 heap의 height와 빼주면 idx의 height가 나오는 아이디어 구상
        return getHeapHeight() - (ceil(log2(idx + 1)) - 1);
    }
    void swap(int idx1, int idx2) { // heap에 입력된값 swap해주는 함수
        int num = v[idx1];
        v[idx1] = v[idx2];
        v[idx2] = num;
    }
};
int main()
{
    ios::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);
    int testCase, inputSize;
    cin >> testCase;
    while (testCase--)
    {
        Heap vec;
        cin >> inputSize;
        int num;
        while (inputSize--)
        {
            cin >> num;
            vec.insert(num); // heap구조조건에 맞게 upheap
        }
        vec.constructHeap(1);
        vec.print();
        while (!vec.isEmpty()) {
            vec.DeleteMAX();
        }
        cout << vec.vacant << endl;
    }
    return 0;
}