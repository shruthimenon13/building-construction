#ifndef REDBLACK__H
#define REDBLACK__H

#include "common.h"

// A class for Min Heap
class Red_black_tree
{

public:
    Red_black_node *root;
    FILE * output_file;

    int building_under_operation;

    // initialize root
    Red_black_tree() { root = NULL; building_under_operation = -1; }

    // Insert
    Red_black_node * insert(int n, RBT_t building_info);

    // Delete the node
    void delete_node(Red_black_node *v);
  
    // Print tree in a nice way for debugging
    void print_entire_tree(Red_black_node *root, Trunk *prev, bool isLeft);

    // Prints (when asked to print between start_building, end_building)
    void Print(int start_building, int end_building);
    void Print(Red_black_node *root, int start_building, int end_building, int init_print_start);

    // Search for a node (also used to error out if building already exists)
    Red_black_node * search_for_node(int n);

    // Get the root node
    Red_black_node *get_root_node() { return root; }

    // Rotate functions
    void rotate_left(Red_black_node *x);
    void rotate_right(Red_black_node *x);

    // Swap functions used during insert, delete
    void swap_colors(Red_black_node *x1, Red_black_node *x2);
    void swap_the_values(Red_black_node *u, Red_black_node *v);

    // Handling error cases
    void handle_red_red_case(Red_black_node *x);
    void handle_black_black_case(Red_black_node *x);

    // This is mostly to find the replacement nodes
    Red_black_node * get_leftmost_successor(Red_black_node *x);
    Red_black_node * find_replacement_node (Red_black_node *x);

    // Just used in printing
    void showTrunks(Trunk *p);

};

#endif
