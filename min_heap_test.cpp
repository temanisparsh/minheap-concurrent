#include <iostream>
//#include <thread>
//#include <shared_mutex>
#include <deque>

using namespace std;

class MinHeap
{

private:
    //std::__shared_mutex_pthread mtx;
    std::deque<int> heap;
    int size = 0;

public:
    void swap(int i, int j)
    {
        //int temp1 = elementAt(i);
        //int temp2 = elementAt(j);
        int temp1 = heap[i];
        int temp2 = heap[j];
        updateElement(i, temp2);
        updateElement(j, temp1);
    }

    void updateElement(int pos, int ele)
    {
        heap[pos] = ele;
    }

    /*int elementAt(int i)
    {
        int ans = heap[i];
        return ans;
    }*/

    void heapify(int i)
    {
        int parent = (i - 1) / 2;
        //if (elementAt(parent) > 0)
        if(heap[parent] > 0)
        {
            //if (elementAt(i) < elementAt(parent))
            if(heap[i] < heap[parent])
            {
                swap(i, parent);
                heapify(parent);
            }
        }
    }

    void insert(int ele)
    {
        //mtx.lock();

        heap.push_back(ele);
        size++;
        heapify(getSize() - 1);

        //mtx.unlock();
    }

    void heapifydown(int i, int hsize)
    {
        int l = 2 * i + 1;
        int r = 2 * i + 2;
        int smallest = i;
        //if (l < hsize && elementAt(l) < elementAt(smallest))
        if(l < hsize && heap[l] < heap[smallest])
            smallest = l;
        //if (r < hsize && elementAt(r) < elementAt(smallest))
        if(r < hsize && heap[r] < heap[smallest])
            smallest = r;
        if (smallest != i)
        {
            swap(i, smallest);
            heapifydown(smallest, hsize);
        }
    }

    void remove()
    {
        //mtx.lock();

        if (size > 0)
        {
            swap(size - 1, 0); //not sure if we can access size directly?
            size--;
            heap.pop_back();
            heapifydown(0, size);
        }

        //mtx.unlock();
    }

    int exists(int ele)
    {
        //mtx.lock_shared();

        int i = 0, hsize = getSize();
        while (i < hsize)
        {
            //if (elementAt(i) == ele)
            if (heap[i] == ele)
                return 1;
            i++;
        }
        return 0;

        //mtx.unlock_shared();
    }

    void print()
    {
        //mtx.lock();

        int hsize = getSize(), i = 0;
        while (i < hsize)
        {
            //cout << elementAt(i) << " ";
            cout << heap[i] << " ";
            i++;
        }
        cout << "\n";

        //mtx.unlock();
    }

    int getSize()
    {
        int res = size;
        return res;
    }
};


int main()
{
    int option, key;
    MinHeap h;
    cin >> option;
    do
    {
        switch(option)
        {
            case 1:
                cin >> key;
                h.insert(key);
                h.print();
                break;

            case 2:
                h.remove();
                h.print();
                break;

            case 3:
                cin >> key;
                cout << h.exists(key) << endl;
                h.print();
                break;

        }
        cin >> option;
    } while(option != 0);
}
