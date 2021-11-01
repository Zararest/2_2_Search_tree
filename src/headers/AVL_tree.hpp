#pragma once

#include <cassert>
#include <iostream>
#include <math.h>

typedef int T_key;

enum Rotations{

    left_rot = 1,
    right_rot = 2,
    big_left_rot = 3,
    big_right_rot = 4,
    none = 5
};

enum Node_errorrs{

    OK = 1,
    left_depth = 2,
    right_depth = 3,
    left_size = 4,
    right_size = 5
};

class Node{

private:

    Node* prev;
    Node* left = nullptr;
    Node* right = nullptr;

    T_key node_key;

    int size_of_left_tree = 0;
    int size_of_right_tree = 0;

    int depth_of_left = 0;
    int depth_of_right = 0;

    int node_error = OK;

public:

    Node(T_key new_key, Node* prev_node = nullptr);

    ~Node();
    
    bool check_balance() const;

    int choose_rotation() const;

    bool check_data();
    
    void left_rotation();

    void right_rotation();

    void big_left_rotation();

    void big_right_rotation();

    void fix_node_data();

    void add_left_node(T_key new_key);

    void add_right_node(T_key new_key);

    int get_error() const{

        return node_error;
    }

    int get_left_tree_size() const{

        return size_of_left_tree;
    }

    int get_right_tree_size() const{

        return size_of_right_tree;
    }

    int get_left_depth() const{

        return depth_of_left;
    }

    int get_right_depth() const{

        return depth_of_right;
    }

    T_key get_key() const{

        return node_key;
    }

    Node* go_left() const{

        return left;
    }

    Node* go_right() const{

        return right;
    }

    Node* go_back() const{

        return prev;
    }

    void delete_right(){

        delete right;
        right = nullptr;
    }

    void delete_left(){

        delete left;
        left = nullptr;
    }

    void increase_size_of_left(){

        size_of_left_tree++;
    }

    void increase_size_of_right(){

        size_of_right_tree++;
    }

    void print_node(std::ostream& outp_stream){
        
		outp_stream << node_key << " [" << depth_of_left << ", " << depth_of_right << "]" << std::endl;
	}
};


class AVL_tree{

protected:

    Node* root = nullptr;
    bool error_occurred = false;
    long number_of_rotations = 0;

    void balance_tree(Node* cur_tree_root);
    Node* find_elems_node(T_key elem) const;

public:

    AVL_tree();

    AVL_tree(const AVL_tree& old_tree);

    ~AVL_tree();

    bool check_add_new_elem_condition(T_key new_key);

    long add_new_elem(T_key new_key);

    bool find_elem(T_key elem) const;

    T_key get_last_elem(int degree_of_last_elem);

    bool check_get_last_elem_condition(int degree_of_last_elem);

    int number_of_elems_less_than(int cur_elem) const;
    
    void dump(std::ostream& outp_stream = std::cout) const;    

    bool check_error();
};