# Concurrent MinHeap in C++

### Steps to run:
1. Clone this repo
2. ```minheap.hpp``` can be directly included so make file is not needed.   
    Test the file by running: 
    ```g++ main.cpp -lpthread```
    ```./a.out```

The ```main.cpp``` contains a few test cases and has 3 different types of instructions:
1. *Insert*: To test this operation push ```1``` in the vector o along with the element to be inserted in vector a.
2. *Remove*: To test this operation push ```2``` in the vector o.
3. *Exists*: To test this operation push ```3``` in the vector o along with the element to be checked in  a.

**Note**: *Remove* operation removes the root Node in the heap.

#### How the library avoid deadlocks:

* Using a mutex for every node in the heap defined in the array ele_mtx. This enables us to control when we allow writes and when the heap nodes are in read only mode. This is implemented using ```lock and shared_lock``` on ```std::__shared_mutex_pthread```.

* Exists function can be excempted as it remains updated with the state of the heap at all points. This is because the exists function fetches the state of the heap in every iteration and immediately locks the size_mtx mutex which avois removal and insertions to the heap temporarily.

* Using a global mutex called size_mtx to avoid insert and remove operations when conflicting with one another.  
Example: In case a removeRoot operation is followed by a insert operation.
In this case, we do not want the insert thread to acces the last element till the swap between the root and the last element occurs. This can be easily implemented using a global mutex.

* To avoid a conflict between heapify and heapifyDown when it is highly likely for them to try and access a common element, however heapify would have that common element as parent and heapifyDown has it as a left / right node. To avoid this, we always try a lock on the child node first irrespective of the implementation. In case of conflicts, both threads try to acquire the common element first and then their current nodes. 

* Using the read write locks along with the previous 2 points not only make it concurrent but also help improve the performance of allowing multiple threads to run in parallel even if they have conflicting operations. The code runs in synchronous mode whenever there is a possible conflicting operation though. To run it in synchronous mode, size_mtx is used to halt the rest of the threads, except the exists function.

### Edge Cases handled in Tests:
* Test case 1:
    Having atleast 2 threads perform different functions - insertion, deletion, search

* Test case 2:
    Deletion and search when no elements are present

* Test case 3:
    Having atleast 2 threads perform different functions - insertion, deletion, search

* Test case 4:
    Having atleast 2 threads perform different functions - insertion, deletion, search

* Test case 5:
    Insertion of elements in a decreasing order with a few deletions and search
