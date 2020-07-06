#include <iostream>
#include <vector>
#include <thread>
#include "minheap.hpp"

// Runs a test case
// for options 1 - insertion, 2 - deletion, 3 - search present in opt
// for corresponding data values in arr
void run_test(std::vector<int>, std::vector<int>, int);

int main()
{
    // Test case 1:
    // Atleast 2 threads perform different functions - insertion, deletion, search considering all edge cases
    std::vector<int> o1 =  {1, 1, 1, 2, 3, 1, 1, 2, 3, 1, 1, 3, 2, 1, 3, 1, 1,  3, 2, 3, 1, 3, 1, 2, 1};
    std::vector<int> a1 =  {5, 7, 2,    2, 3, 8,    4, 8, 8, 8,    9, 1, 1, 3, 10,    8, 1, 8, 4,    7};
    std::cout << "Running test case 1...\n";
    run_test(o1, a1, 25);

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
