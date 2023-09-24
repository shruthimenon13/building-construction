// A class for Min Heap
class MinHeap
{
    MinHeap_t heap_array[MAX_HEAP_SIZE];


public:
    int heap_size; // Current number of elements in min heap

    FILE * output_file;

    // Constructor
    MinHeap();

    // to heapify a subtree with the root at given index
    void MinHeapify(int );

    int get_parent_index(int i) { return (i-1)/2; }

    // to get index of get_left_child_index child of node at index i
    int get_left_child_index(int i) { return (2*i + 1); }

    // to get index of get_right_child_index child of node at index i
    int get_right_child_index(int i) { return (2*i + 2); }

    void print_heap();

    int get_min_element(int parent, int child);


    // to extract the root which is the minimum element
    MinHeap_t remove_root();

    // Decreases key value of key at index i to new_val
    //void decreaseKey(int i, int new_val);

    // Deletes a key stored at index i
    //void delete_element(MinHeap_t i);


    // Inserts a new key 'k'
    void insert_element(MinHeap_t k);
};
