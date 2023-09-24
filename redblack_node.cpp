#ifndef REDBLACK__NODE
#define REDBLACK__NODE

#include "common.h"


// This is used by red black tree during printing  to track the previous node and string
struct Trunk
{
    Trunk *prev;
    string str;
    Trunk(Trunk *prev, string str)
    {
        this->prev = prev;
        this->str = str;
    }
};


class Red_black_node {
public:

  // Note: Val actually always matches the building number
  int val;
  COLOR color;

  // Storing in a struct to make it easy to display
  RBT_t building_info;
  Red_black_node *left, *right, *parent;


  Red_black_node(int val, RBT_t full_entry) {

    // Add the value and the entire building information for displaying anytime
    this -> val = val;
    this -> building_info = full_entry;
    parent = NULL;
    left = NULL;
    right = NULL;

    color = RED; // Since by default the new node is Red color
  }


  // This is mainly to check if this is a left child
  // Used by Red black tree to figure out if this is a triangle case or a line case
  bool is_left_child() {
      if(this == parent->left){
        return true;
      }else{
        return false;
      }
  }



  // This will push this node down.
  // The replacement node (ie, new parent) will now come between this node and its previous parent
  void push_node_down_replacing_with(Red_black_node *new_parent) {
    if (parent != NULL) {
      if (is_left_child()) {
        parent->left = new_parent;
      } else {
        parent->right = new_parent;
      }
    }
    new_parent->parent = parent;
    parent = new_parent;
  }


  // This can be called to return this node's uncle
  Red_black_node *get_uncle() {

    if (parent == NULL or parent->parent == NULL){
      // In this case uncle does not even exist
      return NULL;
    }

    // This means there is an uncle.. So just get it from the grandparent
    if (parent->is_left_child()){
      return parent->parent->right;
    }else{
      return parent->parent->left;
    }
  }


  // This can be called to return this node's brother or sister
  Red_black_node *get_brother() {

    if (parent == NULL){
      // If I don't have a parent I don't have a brother
      return NULL;
    }

    // Else just get the other child of the parent
    if (is_left_child()){
      return parent->right;
    }else {
      return parent->left;
    }
  }


  bool has_red_child() {
    if(left != NULL and left->color == RED){
      return true;
    }else if(right != NULL and right->color == RED){
      return true;
    }else {
      return false;
    }
  }

};

#endif
