#include <iostream>
#include <thread>
#include <shared_mutex>
#include <deque>

using namespace std;

class MinHeap
{

private:
    std::__shared_mutex_pthread mtx;
    std::deque<int> heap;
    int size = 0;

public:

    void swap(int i, int j) {

        int temp1 = elementAt(i);
        int temp2 = elementAt(j);
        updateElement(i, temp2);
        updateElement(j, temp1);

    }

    void updateElement(int pos, int ele) {
        mtx.lock();
        heap[pos] = ele;
        mtx.unlock();
    }

    int elementAt(int i) {
        mtx.lock_shared();
        int ans = heap[i];
        mtx.unlock_shared();
        return ans;
    }

    void heapify(int i)
    {
        int parent = (i - 1) / 2;
        if (elementAt(parent) > 0)
        {
            if (elementAt(i) > elementAt(parent))
            {
                swap(i, parent);
                heapify(parent);
            }
        }
    }

    void insert(int ele)
    {
        mtx.lock();
        heap.push_back(ele);
        size++;
        mtx.unlock();

        heapify(getSize() - 1);
    }

    void heapifydown(int i) {

        int l = 2*i + 1;
        int r = 2*i + 2;
        int smallest = i;
        if(elementAt(l) < elementAt(i))
            smallest = l;
        if(elementAt(r) < elementAt(smallest))
            smallest = r;
        if(smallest != i)
        {
            swap(i, smallest);
            heapifydown(smallest);
        }

    }

    void remove() {

          if(size >= 0)
          {
              mtx.lock();
              heap[0] = heap[getSize() - 1];  //not sure if we can access size directly?
              size--;
              mtx.unlock();
              heapifydown(0);
          }

    }

    int exists(int ele) {

          int i = 0, end = getSize();
          while(i < end) {
              if(elementAt(i) == ele)
                  return 1;
              i++;
          }
          return 0;

    }

    void print() {

          int hsize = getSize(), i = 0;
          while(i < hsize) {
              cout << elementAt(i) << " ";
              i++;
          }
          cout << "\n";
    }

    int getSize()
    {
        mtx.lock_shared();
        int res = size;
        mtx.unlock_shared();
        return res;
    }
};

int main()
{
    return 0;
}
