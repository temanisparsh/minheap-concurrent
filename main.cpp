#include <iostream>
#include <deque>
#include <vector>
#include <thread>
#include <mutex>
#include <shared_mutex>

//Implementation of Min Heap: A complete binary tree where the data
//contained in each node is less than the data on that node's children.
class MinHeap
{
private:

    std::deque<int> heap;   // Stores Min Heap Data Structure
    int size = 0;           // Stores size of heap

    // Read-write mutexes to protect shared data from being simultaneously accessed by multiple threads.
    std::__shared_mutex_pthread size_mtx;
    std::__shared_mutex_pthread print_mtx;
    std::__shared_mutex_pthread ele_mtx[100];

    // Swaps data present at node positions i and j of heap
    void swap(int i, int j)
    {
        int temp1 = heap[i];
        int temp2 = heap[j];
        heap[i] = temp2;
        heap[j] = temp1;
    }

    // Heapify ith node of heap with a bottom-up approach
    void heapify(int i)
    {
        size_mtx.lock_shared();
        // parent index of ith node
        int parent = (i - 1) / 2;

        if (parent >= i || parent < 0)
        {
            size_mtx.unlock_shared();
            return;
        }

        ele_mtx[i].lock();
        ele_mtx[parent].lock();

        // If ith node is greater than its parent, swap and
        // recurively call heapify on parent node
        if (heap[i] < heap[parent])
        {
            swap(i, parent);

            ele_mtx[i].unlock();
            ele_mtx[parent].unlock();

            size_mtx.unlock_shared();

            heapify(parent);
        }
        else
        {
            ele_mtx[i].unlock();
            ele_mtx[parent].unlock();

            size_mtx.unlock_shared();
        }
    }

    // Heapify ith node of heap with top-down approach
    void heapifydown(int i, int hsize)
    {
        size_mtx.lock_shared();

        // Identify index of ith node with the smallest data

        // Index of ith node's left child
        int l = 2 * i + 1;
        // Index of ith node's right child
        int r = 2 * i + 2;
        // Index of the smallest node to be stored
        int smallest = i;

        ele_mtx[l].lock_shared();
        ele_mtx[i].lock_shared();

        // If the left child is smaller than the ith node
        if (l < size && heap[l] < heap[smallest])
            smallest = l;

        ele_mtx[l].unlock_shared();
        ele_mtx[i].unlock_shared();

        ele_mtx[r].lock_shared();
        ele_mtx[i].lock_shared();

        // If the right child is smaller than the ith node
        if (r < size && heap[r] < heap[smallest])
            smallest = r;

        ele_mtx[r].unlock_shared();
        ele_mtx[i].unlock_shared();

        // If the data present on the ith node is not the smallest
        // Swap it with the smallest node's data and recursively heapify
        if (smallest != i)
        {
            ele_mtx[smallest].lock();
            ele_mtx[i].lock();

            swap(i, smallest);

            ele_mtx[smallest].unlock();
            ele_mtx[i].unlock();
            size_mtx.unlock_shared();

            heapifydown(smallest, getSize());
        }
        else
        {
            size_mtx.unlock_shared();
        }
    }

    // Returns the current size of the heap
    int getSize()
    {
        size_mtx.lock_shared();
        int res;
        res = size;
        size_mtx.unlock_shared();
        return res;
    }

public:

    // Deletion of the root node from the heap
    void remove()
    {
        size_mtx.lock();

        int heapSize = size;
        // Deletion when more than 1 node is present in the heap
        if (heapSize > 1)
        {
            // Reduce the size of the heap by 1
            size--;

            ele_mtx[heapSize - 1].lock();
            ele_mtx[0].lock();

            // Swap the root nodes data with the last eelement's data
            swap(0, heapSize - 1);

            ele_mtx[0].unlock();
            ele_mtx[heapSize - 1].unlock();

            // To pop out the root node which is now present as the last element
            heap.pop_back();

            size_mtx.unlock();

            // Heapify(Top-down) the new data present at root node
            heapifydown(0, getSize());
        }
        // Deletion when a single node is present in the heap
        else if (heapSize == 1)
        {
            size = 0;
            heap.pop_back();
            size_mtx.unlock();
        }
        else
            size_mtx.unlock();
    }

    // Searching for a node with data - ele in the heap
    // Prints 1 if ele present, prints 0 if ele is not present
    void exists(int ele)
    {
        int i = 0;
        // Traversing
        while (i < getSize())
        {
            size_mtx.lock_shared();

            // If ele is found, prints 1 and exits
            if (heap[i] == ele)
            {
                size_mtx.unlock_shared();
                print_mtx.lock();
                std::cout << "1 ";
                print_mtx.unlock();
                return;
            }
            size_mtx.unlock_shared();
            i++;
        }

        // If ele is not found, prints 0
        print_mtx.lock();
        std::cout << "0 ";
        print_mtx.unlock();

    }

    // Prints data in each node present in the heap
    void print()
    {
        print_mtx.lock();
        int hsize = size;
        int i = 0;
        if(hsize == 0)
        {
            std::cout << "empty\n";
            print_mtx.unlock();
            return;
        }
        while (i < hsize)
        {
            std::cout << heap[i] << " ";
            i++;
        }
        std::cout << "\n";
        print_mtx.unlock();
    }

    // Inserts node with data ele into the heap
    void insert(int ele)
    {

        size_mtx.lock();

        // Inserts node with data ele to the last node of the heap
        heap.push_back(ele);

        // Increases size of heap by 1
        int newSize = ++size;

        size_mtx.unlock();

        // Heapify(Bottom-up) the new node into the heap
        heapify(newSize - 1);
    }

    // Checks whether all the nodes in the heap follows the heap property or not
    // Returns 1 if heap property is followed, returns 0 otherwise
    int isHeap()
    {

        for (int i = 0; i < size; i++) {
            // If value of child node is smaller than the parent node
            // Min heap property is not satisfied and 0 is returned
            if (heap[i] < heap[(i-1)/2]) {
                std::cout << i;
                return 0;
            }
        }
        // If all nodes follow the heap property, 1 is returned
        return 1;
    }

};

// Runs a test case
// for options 1 - insertion, 2 - deletion, 3 - search present in opt
// for corresponding data values in arr
void run_test(std::vector<int> opt, std::vector<int> arr, int opt_size)
{

    MinHeap heap;
    std::thread threads[opt_size];

    int o= 0, a=0;

    // Spawning threads for all options in opt
    while(o < opt_size)
    {
        if(opt[o] == 1)
        {
            threads[o] = std::thread(&MinHeap::insert, &heap, arr[a]);
            a++;
        }
        else if(opt[o] == 2)
        {
            threads[o] = std::thread(&MinHeap::remove, &heap);
        }
        else
        {
            threads[o] = std::thread(&MinHeap::exists, &heap, arr[a]);
            a++;
        }
        o++;
    }

    // Joining the threads
    for(int i = 0; i < opt_size; i++)
        threads[i].join();

    // Printing the final heap's contents after the completion of thread execution
    std::cout << "\nfinalHeap: ";
    heap.print();
    // Printing whether the final heap satisfies whether all the nodes satisfy the min heap property
    std::cout << "isHeap: " << heap.isHeap() << "\n\n";

}

int main()
{
    // Test case 1:
    // Atmost 2 threads perform different functions - insertion, deletion, search
    std::vector<int> o1 =  {1, 1, 1, 2, 3, 1, 1, 2, 3, 1};
    std::vector<int> a1 =  {5, 7, 2,    2, 3, 8,    4, 8};
    std::cout << "Running test case 1...\n";
    run_test(o1, a1, 10);

    // Test case 2:
    // Deletion and search when no elements are present
    std::vector<int> o2 =  {2, 2, 3, 2, 3, 2, 2, 3, 3, 2};
    std::vector<int> a2 =  {      1,    2,       1, 2};
    std::cout << "Running test case 2...\n";
    run_test(o2, a2, 10);

    // Test case 3:
    // Atleast 2 threads perform different functions - insertion, deletion, search
    std::vector<int> o3 =  {2, 3, 1, 1, 2, 3, 1, 1, 2, 3, 1, 1,  1,  3, 2, 2, 2, 1,  1,  1, 3, 3, 1, 2, 1};
    std::vector<int> a3 =  {   7, 7, 2,    2, 3, 8,    4, 8, 1, 17, 11,          4, 12, 11, 4, 9, 5,    6};
    std::cout << "Running test case 3...\n";
    run_test(o3, a3, 25);

    // Test case 4:
    // Atleast 2 threads perform different functions - insertion, deletion, search
    std::vector<int> o4 =  {2, 2, 3, 2, 3, 1, 1, 2, 3, 1, 1,  1, 3, 3, 1, 1, 2, 3, 1, 1, 2, 2, 2, 2,  3};
    std::vector<int> a4 =  {      1,    2, 1, 2,    7, 8, 9, 10, 2, 4, 5, 7,    7, 3, 1,             10};
    std::cout << "Running test case 4...\n";
    run_test(o4, a4, 25);

    // Test case 5:
    // Insertion of elements in decreasing order, with a few deletions and serach
    std::vector<int> o5 =  { 1, 1,  1, 2,  3,  1, 1, 2, 3, 1, 1,  3, 1, 1, 2};
    std::vector<int> a5 =  {17, 12, 11,    12, 9, 8,    8, 6, 5,  4, 4, 3};
    std::cout << "Running test case 5...\n";
    run_test(o5, a5, 15);

    return 0;
}
