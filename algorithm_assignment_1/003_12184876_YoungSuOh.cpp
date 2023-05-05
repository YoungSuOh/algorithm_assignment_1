#include<iostream>
#include<string>
#include<vector>
#include<cmath>
#define endl "\n"
using namespace std;

class Heap // Heap class ����
{
private:
    vector<int>v; // vector�� ���Ƿν� 
    int heapSize; // heapSize ��������� ����
public:
    int vacant = 0; // vacant�� �ʱ⿡ 0���� ���� -> total time�� count�ϱ� ���� ����
    Heap()
    {
        v.push_back(-1); // �θ�� �ڽİ��� ���踦 �ι�� �����ϱ� ���� index 0���� ������ ���� ����ְ� ������� �ʴ´�.
        this->heapSize = 0; // �ʱ⿡ heapSize�� 0���� ����
    }
    ~Heap() {} // �Ҹ��� �Լ�
    void constructHeap(int root_idx) // heap ����� �Լ�
    {
        if (getCurrentHeight(root_idx) != 0) { // leaf�� �ƴҰ�쿡 �ش� ���ǹ� ����
            constructHeap(2 * root_idx); // root_idx�� root�� left subtree�� �̵�
            constructHeap(2 * root_idx + 1); // root_idx�� root�� right subtree�� �̵�
            int k = root_idx;
            downheap(k); // ��͸� ���ֹǷν� heap�� leaf���� �̵��Ͽ� heap�� �������ǿ� �����ϵ��� downheap ����
        }
        return;
    }
    void upheap(int idx) // �ڽİ� �θ� ���ϸ鼭 heap�� ���������� ������Ű�� upheap �Լ�
    {
        if (idx <= 1)
            return;
        else if (v[idx] > v[idx / 2]) // �ڽ��� �� ũ�� swap => MAXHEAP
        {
            vacant++; // vacant�� parent������ ��������Ƿ� 1�߰�
            swap(idx, idx / 2);
            upheap(idx / 2); // ��� ����
        }
    }
    void downheap(int idx) // ���ڽİ��� ũ�⸦ ���Ͽ� ū �ڽ��� �θ�� ���ϴ� downheap�Լ�
    {
        int max; // �� �ڽ� �� ũ�Ⱑ ū �ڽ��� index �ӽ� ���� max�� ����
        if (2 * idx + 1 <= heapSize) // ������ �ڽ��� ���� ����� ���ǹ� -> �Ҵ�� �޸� ������ ���� ��츦 ����(segmentFault)
        {
            if (v[2 * idx] > v[2 * idx + 1]) // ������ �ڽ��� Ŭ���
                max = 2 * idx; // ������ �ڽ��� index���� max�� ����
            else // ���� �ڽ��� Ŭ���
                max = 2 * idx + 1; // ���� �ڽ��� index���� max�� ����
            if (v[idx] < v[max]) // ū �ڽ��� ���� �θ𺸴� Ŭ ��� swap�� downheap ���
            {
                swap(idx, max);
                downheap(max);
            }
            return;
        }
        else if (2 * idx == heapSize) // ���� �ڽĸ� �������
        {
            if (v[idx] < v[2 * idx]) // ���� �ڽİ� ���Ͽ� �����ڽ��� ũ�� �θ�� swap
            {
                swap(idx, 2 * idx);
            }
            return;
        }
    }
    void downheap_H1(int current) { // fixHeapfast�� ������ ��� height�� 1�϶� vacant���� ���� ���� �Լ�, �ϴ� ������ downheap�� ����
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
    void fixHeapFast(int current, int height, int key) // accelerate heapsort�� ���� fixHeapSort
    {
        if (height <= 1) { // ���̰� 1�ϰ�쿡�� downheap_H1�� �����Ѵ�.
            downheap_H1(current);
            return;
        }
        else {
            int hstop = height / 2; // height/2���� �������� ����
            int vacStop = promote(hstop, current); // height/2���� key�� �ڽĵ��� ū�ڽİ� swap�ϰ� promote�Լ� return�� vacStop�� ����
            int vacParent = vacStop / 2; // vacparent index ����
            if (v[vacParent] <= v[vacStop]) { // height/2���� ������ �������� parent�� ���Ͽ� parent���� ���� ��ġ�� data���� �� ũ��
                swap(vacParent, vacStop); // parent�� swap
                vacant++;  // vacant�� �̵������Ƿ� +1 �߰�
                upheap(vacParent); // heap ���������� �����ϱ� ���� upheap����
            }
            else
                fixHeapFast(vacStop, hstop, key); // height�� 0�Ǵ� 1�� �ɶ����� ��� ����
        }
        return;
    }
    int promote(int hstop, int current) // height/2���� �̵��ϸ鼭 �ڽĵ� �� ū�ڽİ� key�� swap���ִ� �Լ�
    {
        int vacstop;
        if (getCurrentHeight(current) <= hstop) { // ����� �ϴ� ��ġ hstop�� ���������� 
            vacstop = current; // ���߰� �ش� ��ġ�� vacStop
        }
        else if (v[2 * current] <= v[2 * current + 1]) { // ������ �ڽ��� ū���
            swap(current, 2 * current + 1); // ������ �ڽİ� swap
            vacstop = promote(hstop, 2 * current + 1); // ���� ������ϴ� ��ġ���� ��� ����
            vacant++; // vacant��ġ �ٲ�����Ƿ� vacant +1
        }
        else { // ���� �ڽ��� ū ���
            swap(current, 2 * current); // �����ڽİ� swap
            vacstop = promote(hstop, 2 * current); // ���� ������ϴ� ��ġ���� ��� ����
            vacant++; // vacant��ġ �ٲ�����Ƿ� vacant +1
        }
        return vacstop;
    }
    void insert(int data) // heap�� data�� ���������� �־��ִ� �Լ�
    {
        heapSize++; // data�� ���������� heapSize �ϳ��� ����
        v.push_back(data);
    }
    bool isEmpty() // heap�� ������� �ƴ��� Ȯ���Ͽ� true / false�� ��ȯ���ִ� �Լ�
    {
        return heapSize == 0;
    }
    void DeleteMAX() // index 1�� �ִ� ���� ���ְ� index�� ���� �ִ� �� index 1������ �����Ͽ� accelerate heapsort ���ִ� �Լ�
    {
        if (isEmpty()) // heap�� ����ִٸ� return
            return;
        else
        {
            v[1] = v[heapSize]; // index 1�� �ִ� ���� ���ְ� index�� ���� �ִ� �� index 1������ ����
            v.pop_back(); // index ���� �� ������
            heapSize--; // ���� index 1�� �ִ� �� ���������Ƿ� heapSize -1
            if (isEmpty()) { return; } // �������� heap�� ���� �ϳ� ���� �� ���� ������ �������� case
            else {
                vacant++; // index�� ���� �ִ� �� index 1������ ���������Ƿ� vacant +1
                fixHeapFast(1, getHeapHeight(), v[1]); // accelerate heapsort ����
            }
            return;
        }
    }
    void print() // ���� heap�� �ִ� �� ������ִ� �Լ�
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
    int getHeapHeight() { // ceil(log2(heapSize+1))-1 ���� heapSize�� �־��ֹǷν� heap�� height�� ��ȯ�� �� �ִ�.
        return (ceil(log2(heapSize + 1)) - 1);
    }
    int getCurrentHeight(int idx) { // idx�� height�� �����ִ� �Լ� -> ceil(log2(idx+1))-1 �� idx�� depth�� ��Ÿ���ְ� �̰��� heap�� height�� ���ָ� idx�� height�� ������ ���̵�� ����
        return getHeapHeight() - (ceil(log2(idx + 1)) - 1);
    }
    void swap(int idx1, int idx2) { // heap�� �ԷµȰ� swap���ִ� �Լ�
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
            vec.insert(num); // heap�������ǿ� �°� upheap
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