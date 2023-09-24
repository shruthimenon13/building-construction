
#include "redblack_tree.h"

// ===============================================================
// Handle Insert : Insert the new node into the Red black tree
//===============================================================

Red_black_node * Red_black_tree::insert(int n, RBT_t building_info) {

  // Create the new node

  Red_black_node *new_red_black_node = new Red_black_node(n, building_info);

  if (root == NULL) {

    new_red_black_node->color = BLACK;  // We need the root to be black
    root = new_red_black_node;

  } else {
    // ie, there already exists a root

    // Find the ideal place to insert by searching
    // We also need to make sure there is no duplicate
    Red_black_node *parent = search_for_node(n);

    if (parent->val == n) {
      printf("ERROR: Cannot add %d because this building already exists \n",n);
      fprintf(output_file, "ERROR: Cannot add %d because this building already exists \n",n);
      exit(0);
      return new_red_black_node;
    }

    // Add to the parent
    if (parent->val > n){
      parent->left = new_red_black_node;
    }else {
      parent->right = new_red_black_node;
    }

    // Add parent info to the new node
    new_red_black_node->parent = parent;

    // If there is a case where there is red - red violation, fix it
    handle_red_red_case(new_red_black_node);
  }

  return new_red_black_node;
}


// ===============================================================
// Delete: Delete any node present in the red black tree
//===============================================================

// deletes the given node
void Red_black_tree::delete_node(Red_black_node *v) {

  Red_black_node *u = find_replacement_node(v);
  Red_black_node *parent = v->parent;
  bool u_and_v_are_black = ((v->color == BLACK) and (u == NULL or u->color == BLACK));
  //printf("u = %p, v = %p, parent = %p \n", (void *)u, (void *)v, (void *)parent);

  if (u == NULL) {
    //ie, handle case where the node to delete is the leaf node
    if (v == root) {
        root = NULL; // v is root, making root null
    } else {
      if (u_and_v_are_black) {
        handle_black_black_case(v);  // Fix the black black case
      } else {
        // ie, this is not a black black case
        if (v->get_brother() != NULL){
          v->get_brother()->color = RED;
        }
      } // u_and_v_are_black
      // Set the parent's child as null
      if (v->is_left_child()) {
        parent->left = NULL;
      } else {
        parent->right = NULL;
      }
    } // v is root
    //printf("deleting v = %p \n", (void *)v);
    delete v;
    return;
  }

  // If the node's child is null
  if (v->left == NULL or v->right == NULL) {
    if (v == root) {
      // If v is root, assign the value of u to v, and delete u
      v->val = u->val;
      v->left = v->right = NULL;
      // Since we are updating v, also infom the minheap tree so that this is updated
      v->building_info = u -> building_info;

      if((v -> building_info.building_num) != building_under_operation){
          MinHeap_t * minheap_node_ptr = (v -> building_info).minheap_node_ptr;
          minheap_node_ptr -> rbt_node_ptr = v;
          #if ENA_DEBUG
            printf("RBT1: Changing address of building %d to use RBT=%p because of swap. MinHeapAddress changed = %p \n", v -> building_info.building_num, (void *)v, (void *)minheap_node_ptr);
          #endif
      }

      delete u;
    } else {
      // Detach v from tree
      // Make u the v's parent's child
      if (v->is_left_child()) {
        parent->left = u;
      } else {
        parent->right = u;
      }
      //printf("deleting vv = %p \n", (void *)v);
      delete v;
      u->parent = parent;
      if (u_and_v_are_black) {
        handle_black_black_case(u);
      } else {
        // u or v red, color u black
        u->color = BLACK;
      }
    }
    return;
  }
  // if it came here that means v has two children, so swap the values & delete the node
  // When we swap, also inform the minheap tree so that that is okay
  swap_the_values(u, v);
  delete_node(u);

}


// ================================================================================
// This is to print the tree in a nice way. This can be used for debugging anytime.
// This prints the building number and the current address of this node
// ================================================================================
void Red_black_tree::print_entire_tree(Red_black_node *root, Trunk *prev, bool isLeft)
{
    if (root == nullptr) return;
    string prev_str = "    ";
    Trunk *trunk = new Trunk(prev, prev_str);
    print_entire_tree(root->left, trunk, true);
    if (!prev)
        trunk->str = "---";
    else if (isLeft){
        trunk->str = ".---";
        prev_str = "   |";
    }else{
        trunk->str = "`---";
        prev->str = prev_str;
    }
    showTrunks(trunk);
    RBT_t building_info = root -> building_info;
    printf("(%d, RBT=%p, HEAP=%p) \n",building_info.building_num, (void *)root, (void *)building_info.minheap_node_ptr);
    if (prev)
        prev->str = prev_str;
    trunk->str = "   |";
    print_entire_tree(root->right, trunk, false);
}

// ================================================================================
// print all nodes between the start and end buildings
// ================================================================================

volatile int prints_done = 0;
void Red_black_tree::Print(int start_building, int end_building){
    Print(root, start_building, end_building, 1);
    if(prints_done == 0){
      fprintf(output_file, "(0,0,0)");
    }
    fprintf(output_file, "\n");
}

void Red_black_tree::Print(Red_black_node *root, int start_building, int end_building, int init_print_start)
{
      if(init_print_start) {
        prints_done = 0;
      }

      if ( NULL == root ) return;

      // Start with the left subtree so that we can print in the increasing order
      if ( start_building < root->val ){
          Print(root->left, start_building, end_building,0);
      }

      // Next check if this node itself needs to be printed or not
      if ( start_building <= root->val && end_building >= root->val ){
          RBT_t building_info = root -> building_info;
          if(prints_done != 0){
              fprintf(output_file, ",");
          }
          prints_done ++;
          fprintf(output_file, "(%d,%d,%d)",building_info.building_num, building_info.execution_time, building_info.total_time);
      }

      // Go to the right subtree if there is any node that can fall there (ie, end is greater than this node)
      if ( end_building > root->val )
          Print(root->right, start_building, end_building,0);
}


// =================================================================================
// Search for the correct position where the node is located or needs to be inserted
// This is used by the insert function to also check for duplicates if any and error out
// =================================================================================

Red_black_node * Red_black_tree::search_for_node(int n) {
  Red_black_node *current_node = root;
  while (current_node != NULL) {
    if (n < current_node->val) {
      if (current_node->left == NULL)
        break;
      else
        current_node = current_node->left;
    } else if (n == current_node->val) {
      break;
    } else {
      if (current_node->right == NULL)
        break;
      else
        current_node = current_node->right;
    }
  }
  return current_node;
}


// =================================================================================
// All rotations used during insert, delete
// =================================================================================

void Red_black_tree::rotate_left(Red_black_node *x) {

    // This means the parent will be the node's right child
    Red_black_node *new_parent = x->right;

    // Update root if needed
    if (x == root){
        root = new_parent;
    }

    // Do connections with parents
    x->push_node_down_replacing_with(new_parent);
    x->right = new_parent->left;
    if (new_parent->left != NULL) {
      new_parent->left->parent = x;
    }
    new_parent->left = x;
}

void Red_black_tree::rotate_right(Red_black_node *x) {

  // This means the parent will be the node's left child
    Red_black_node *new_parent = x->left;

    // update root if needed
    if (x == root){
      root = new_parent;
    }

    // Same as in left case... connect with parents
    x->push_node_down_replacing_with(new_parent);
    x->left = new_parent->right;
    if (new_parent->right != NULL){
      new_parent->right->parent = x;
    }
    new_parent->right = x;
}



// ===================================================================
// All kids of swaps.. ie, colors, values used in insertion, ideletion
// ===================================================================

void Red_black_tree::swap_colors(Red_black_node *x1, Red_black_node *x2) {
    COLOR temp;
    temp = x1->color;
    x1->color = x2->color;
    x2->color = temp;
}

void Red_black_tree::swap_the_values(Red_black_node *u, Red_black_node *v) {
    int temp;
    RBT_t building_info_temp;

    temp = u->val;
    building_info_temp = u -> building_info;

    u->val = v->val;
    u -> building_info = v -> building_info;

    v->val = temp;
    v-> building_info = building_info_temp;

    // Since we are updating v, also infom the minheap tree so that this is updated
    if((u -> building_info.building_num) != building_under_operation){
      MinHeap_t * minheap_node_ptr_u = (u -> building_info).minheap_node_ptr;
      minheap_node_ptr_u -> rbt_node_ptr = u;
      #if ENA_DEBUG
        printf("RBT2: Changing address of building %d to use RBT=%p because of swap. MinHeapAddress changed = %p \n", u -> building_info.building_num, (void *)u, (void *)minheap_node_ptr_u);
      #endif
    }

    if((v -> building_info.building_num) != building_under_operation){
      MinHeap_t * minheap_node_ptr_v = (v -> building_info).minheap_node_ptr;
      minheap_node_ptr_v -> rbt_node_ptr = v;
      #if ENA_DEBUG
        printf("RBT3: Changing address of building %d to use RBT=%p because of swap. MinHeapAddress changed = %p \n", v -> building_info.building_num, (void *)v, (void *)minheap_node_ptr_v);
      #endif
    }

    #if ENA_DEBUG
      print_entire_tree(root,NULL,false);
    #endif
}



// ===================================================================
// Handling violation cases (red red, black black etc)
// ===================================================================



// fix red red at given node
void Red_black_tree::handle_red_red_case(Red_black_node *x) {

    if (x == root) {
      // This is the simplest case, just color it black
      x->color = BLACK;
      return;
    }

    //
    Red_black_node *parent = x->parent;
    Red_black_node *grandparent = parent->parent;
    Red_black_node *uncle = x->get_uncle();

    if (parent->color != BLACK) {
      if (uncle != NULL && uncle->color == RED) {
        parent->color = BLACK;
        uncle->color = BLACK;
        grandparent->color = RED;
        handle_red_red_case(grandparent);
      } else {

        // Now handle all cases where it forms a triangle or a line
        // So first we need to check if this is a left child of a left parent => Line
        // Then right child of left parent   => Triangle
        // Then left child of right parent => Triangle
        // Then right child of right parent  => Line

        if (parent->is_left_child()) {

          if (x->is_left_child()) {
            //left child of a left parent
            swap_colors(parent, grandparent);
          } else {
            // right child of left parent
            rotate_left(parent);
            swap_colors(x, grandparent);
          }
          rotate_right(grandparent);

        } else {

          if (x->is_left_child()) {
            // left child of right parent
            rotate_right(parent);
            swap_colors(x, grandparent);
          } else {
            //right child of right parent
            swap_colors(parent, grandparent);
          }
          rotate_left(grandparent);
        } // parent is left child
      } // uncle is red
    } // parents color is black
  } // red red case




void Red_black_tree::handle_black_black_case(Red_black_node *x) {
    if (x == root){
        return;
    }

    Red_black_node *brother = x->get_brother();
    Red_black_node *parent = x->parent;

    if (brother == NULL) {
        handle_black_black_case(parent);

    } else {

        if (brother->color == RED) {
          parent->color = RED;
          brother->color = BLACK;
          if (brother->is_left_child()) {
            rotate_right(parent);
          } else {
            rotate_left(parent);
          }
          handle_black_black_case(x);
        } else {
          if (brother->has_red_child()) {
            if (brother->left != NULL and brother->left->color == RED) {
              if (brother->is_left_child()) {
                brother->left->color = brother->color;
                brother->color = parent->color;
                rotate_right(parent);
              } else {
                brother->left->color = parent->color;
                rotate_right(brother);
                rotate_left(parent);
              }
            } else {
              if (brother->is_left_child()) {
                brother->right->color = parent->color;
                rotate_left(brother);
                rotate_right(parent);
              } else {
                brother->right->color = brother->color;
                brother->color = parent->color;
                rotate_left(parent);
              }
            }
            parent->color = BLACK;
          } else {
            brother->color = RED;
            if (parent->color == BLACK)
              handle_black_black_case(parent);
            else
              parent->color = BLACK;
          } // has red child
        } // brothers color is red
      } // there is a brother
  }






  // This is mainly to find the replacement if we are going to delete the tree

  Red_black_node * Red_black_tree::find_replacement_node (Red_black_node *x) {

    // when node have 2 children
    if (x->left != NULL and x->right != NULL){
      // We find the leftmost child in the right subtree
      return get_leftmost_successor(x->right);
    }

    if (x->left == NULL and x->right == NULL){
      // This is just the leaf node... so return NULL
      return NULL;
    }

    // If t comes here this means it has one child. So just return that child
    if (x->left != NULL){
      return x->left;
    }else {
      return x->right;
    }
  }



// This is just to find the leftmost child (ie, lowest value child)
// This is used to find the replacement node
Red_black_node * Red_black_tree::get_leftmost_successor(Red_black_node *x) {
    Red_black_node *current_node = x;
    while (current_node->left != NULL){
      current_node = current_node->left;
    }
    return current_node;
}

// This is just a function call used for printing in a nice way for debug purpose
void Red_black_tree::showTrunks(Trunk *p)
{
      if (p == nullptr) {
          return;
      }
      showTrunks(p->prev);
      cout << p->str;
}
