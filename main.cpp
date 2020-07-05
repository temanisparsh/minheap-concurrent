#include <iostream>
#include <deque>
#include <vector>
#include <thread>
#include <mutex>
#include <shared_mutex>

class MinHeap
{
private:
    std::deque<int> heap;
    int size = 0;

    std::__shared_mutex_pthread size_mtx;
    std::__shared_mutex_pthread print_mtx;
    std::__shared_mutex_pthread ele_mtx[100];

    void swap(int i, int j)
    {
        int temp1 = heap[i];
        int temp2 = heap[j];
        heap[i] = temp2;
        heap[j] = temp1;
    }

    void heapify(int i)
    {
        int parent = (i - 1) / 2;
        if (parent >= i)
            return;
        ele_mtx[i].lock();
        ele_mtx[parent].lock();
        if (heap[parent] > 0)
        {
            if (heap[i] < heap[parent])
            {
                swap(i, parent);
                ele_mtx[i].unlock();
                ele_mtx[parent].unlock();
                heapify(parent);
            }
            else
            {
                ele_mtx[i].unlock();
                ele_mtx[parent].unlock();
            }
        }
        else
        {
            ele_mtx[i].unlock();
            ele_mtx[parent].unlock();
        }
    }

    void heapifydown(int i, int hsize)
    {
        int l = 2 * i + 1;
        int r = 2 * i + 2;
        int smallest = i;

        ele_mtx[i].lock_shared();
        ele_mtx[l].lock_shared();

        if (l < hsize && heap[l] < heap[smallest])
            smallest = l;

        ele_mtx[l].unlock_shared();

        ele_mtx[r].lock_shared();

        if (r < hsize && heap[r] < heap[smallest])
            smallest = r;

        ele_mtx[r].unlock_shared();
        ele_mtx[i].unlock_shared();

        if (smallest != i)
        {
            ele_mtx[smallest].lock();
            ele_mtx[i].lock();
            swap(i, smallest);
            ele_mtx[smallest].unlock();
            ele_mtx[i].unlock();
            heapifydown(smallest, hsize);
        }
    }

    int getSize()
    {
        size_mtx.lock_shared();
        int res;
        res = size;
        size_mtx.unlock_shared();
        return res;
    }

public:
    void remove()
    {
        size_mtx.lock();
        int heapSize = size;
        if (heapSize > 0)
        {
            size--;
            heap.pop_back();
            size_mtx.unlock();
            heapifydown(0, size);
        }
        if (heapSize <= 0)
            size_mtx.unlock();
    }

    int exists(int ele)
    {
        int i = 0;
        while (i < getSize())
        {
            ele_mtx[i].lock_shared();
            if (heap[i] == ele)
                ele_mtx[i].unlock_shared();
            return 1;
            i++;
        }
        ele_mtx[i].unlock_shared();
        return 0;
    }

    void print()
    {
        print_mtx.lock();
        int hsize = size;
        int i = 0;
        while (i < hsize)
        {
            std::cout << heap[i] << " ";
            i++;
        }
        std::cout << "\n";
        print_mtx.unlock();
    }

    void insert(int ele)
    {
        size_mtx.lock();

        heap.push_back(ele);
        
        int newSize = ++size;

        size_mtx.unlock();
        heapify(newSize - 1);
    }

    int isHeap() {      

        for (int i = 0; i < size; i++) {
            if (heap[i] < heap[(i-1)/2]) {
                std::cout << i;
                return 0;
            }
        }
        return 1;
    }
};

int main()
{

    MinHeap heap;
    std::thread threads[22];

    int arr[] = {2, 4, 6, 8, 8, 5, 0, 9, 7, 5, 5, 8, 9, 4, 3, 9, 0, 10, 34, 6, 98, 76};

    for(int i = 0; i < 22; i++) {
        threads[i] = std::thread(&MinHeap::insert, &heap, arr[i]);
    }

    for(int i = 0; i < 22; i++) {
        threads[i].join();
    }

    heap.print();
    std::cout << "isHeap: " << heap.isHeap() << '\n';

    return 0;
}