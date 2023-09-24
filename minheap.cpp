
#include "common.h"
#include "minheap.h"


void swap(MinHeap_t *x, MinHeap_t *y);



// Constructor: Builds a heap from a given array a[] of given size
MinHeap::MinHeap()
{
    heap_size = 0;
}

// Inserts a new key 'k'
void MinHeap::insert_element(MinHeap_t element)
{
    if (heap_size == MAX_HEAP_SIZE) {
        printf("ERROR: Cannot add more elements. Current heap_size = %d", heap_size);
        return;
    }

    // First insert the new key at the end
    heap_size++;
    int index = heap_size - 1;
    heap_array[index] = element;

    // Fix the min heap property if it is violated
    while(1){
        int parent_index = get_parent_index(index);
        int do_swap = 0;
        if(index == 0) break;

        if(heap_array[index].execution_time < heap_array[parent_index].execution_time){
            do_swap = 1;
        }

        if((heap_array[get_parent_index(index)].execution_time == heap_array[index].execution_time) &&
                 (heap_array[get_parent_index(index)].building_num > heap_array[index].building_num)) {
            do_swap = 1;
        }

        if(do_swap == 1){
          swap(&heap_array[index], &heap_array[get_parent_index(index)]);
          index = get_parent_index(index);
        }else{
          break;
        }
    }
}

void MinHeap::print_heap(){
  for(int i =0; i < heap_size; i++) {
        printf(" index: %d => execution_time=%d, building_num=%d \n", i, heap_array[i].execution_time, heap_array[i].building_num);
  }
}

/*
// Decreases value of key at index 'i' to new_val.  It is assumed that
// new_val is smaller than heap_array[i].
void MinHeap::decreaseKey(int i, int new_val)
{
    heap_array[i] = new_val;
    while (i != 0 && heap_array[get_parent_index(i)] > heap_array[i])
    {
       swap(&heap_array[i], &heap_array[get_parent_index(i)]);
       i = get_parent_index(i);
    }
}
*/

// Method to remove minimum element (or root) from min heap
MinHeap_t MinHeap::remove_root()
{
    MinHeap_t empty_heap;
    MinHeap_t return_element;
    empty_heap.building_num = -1;

    switch(heap_size){
        case 0 :  return_element = empty_heap;
                  break;

        case 1 :  return_element = heap_array[0];
                  //printf("Return element address = %p", &heap_array[0]);
                  heap_size --;
                  break;

        default:  return_element = heap_array[0];
                  //printf("Return element address = %p", &heap_array[0]);
                  heap_array[0] = heap_array[heap_size-1];

                  Red_black_node * rbt_node_ptr_y = heap_array[0].rbt_node_ptr;
                  (rbt_node_ptr_y -> building_info).minheap_node_ptr = &heap_array[0];

                  #if ENA_DEBUG
                  printf("MINHEAP: Changing address of building %d to use HEAP_PTR=%p because of remove_root. RBT pointer changed = %p \n", heap_array[0].building_num, (void *)&heap_array[0], (void *)rbt_node_ptr_y);
                  #endif 

                  heap_size --;
                  MinHeapify(0);
    }

    return return_element;
}


// This function deletes key at index i. It first reduced value to minus
// infinite, then calls remove_root()
// void MinHeap::delete_element(int i)
//{
    //decreaseKey(i, INT_MIN);/
//    remove_root();
//}

// Method to recompute heap starting at index i
void MinHeap::MinHeapify(int i)
{
    int l = get_left_child_index(i);
    int r = get_right_child_index(i);

    int smallest_element_index;
    smallest_element_index = get_min_element(i,l);
    smallest_element_index = get_min_element(smallest_element_index,r);

    if (smallest_element_index != i)
    {
        swap(&heap_array[i], &heap_array[smallest_element_index]);
        MinHeapify(smallest_element_index);
    }
}

int MinHeap::get_min_element(int parent, int child){
    if(child < heap_size){
      if(heap_array[parent].execution_time > heap_array[child].execution_time){
          return child;
      }
      if((heap_array[parent].execution_time == heap_array[child].execution_time) &&
         (heap_array[parent].building_num > heap_array[child].building_num)){
          return child;
      }
    }
    return parent;
}





// A utility function to swap two elements
void swap(MinHeap_t *x, MinHeap_t *y)
{
    MinHeap_t temp = *x;
    *x = *y;
    *y = temp;

    // Since we are swapping values, let us also inform the RBT about the pointer updates

    Red_black_node * rbt_node_ptr_x = (*x).rbt_node_ptr;
    (rbt_node_ptr_x -> building_info).minheap_node_ptr = x;
    #if ENA_DEBUG
      printf("MINHEAP: Changing address of building %d to use HEAP_PTR=%p because of swap. RBT pointer changed = %p \n", x->building_num, (void *)x, (void *)rbt_node_ptr_x);
    #endif

    Red_black_node * rbt_node_ptr_y = (*y).rbt_node_ptr;
    (rbt_node_ptr_y -> building_info).minheap_node_ptr = y;
    #if ENA_DEBUG
      printf("MINHEAP: Changing address of building %d to use HEAP_PTR=%p because of swap. RBT pointer changed = %p \n", y->building_num, (void *)y, (void *)rbt_node_ptr_y);
    #endif
}
