#include "headers/AVL_tree.hpp"

int max(int fir_num, int sec_num){

    if (fir_num > sec_num){

        return fir_num;
    } else{

        return sec_num;
    }
}

Node::Node(T_key new_key, Node* prev_node): node_key(new_key), prev(prev_node){

    left = nullptr;
    right = nullptr;

    size_of_left_tree = 0;
    size_of_right_tree = 0;

    depth_of_left = 0;
    depth_of_right = 0;

    node_error = OK;
}

Node::~Node(){

    prev = nullptr; //дополнительная проверка поскольку есть вероятность обращения по указателю на мертвого попугая
    left = nullptr;
    right = nullptr;
}

bool Node::check_balance() const{

    if ((depth_of_left - depth_of_right > 1) || (depth_of_left - depth_of_right < -1)){ 

        return false;
    } else{

        return true;
    }
}

int Node::choose_rotation() const{

    if (check_balance() == true){

        return none;
    }

    if (depth_of_left - depth_of_right > 1){

        if (left->depth_of_left - left->depth_of_right >= 0){

            return left_rot;
        } else{

            return big_left_rot;
        }
    } else{

        if (right->depth_of_right - right->depth_of_left >= 0){

            return right_rot;
        } else{

            return big_right_rot;
        }
    }
}

bool Node::check_data(){

    if (left != nullptr){

        if (depth_of_left - 1 != max(left->depth_of_left, left->depth_of_right)){

            node_error = left_depth;
            return false;
        }

        if (size_of_left_tree - 1 != left->size_of_left_tree + left->size_of_right_tree){

            node_error = left_size;
            return false;
        }
    } else{

        if (size_of_left_tree != 0){

            node_error = left_size;
            return false;
        }

        if (depth_of_left != 0){

            node_error = left_depth;
            return false;
        }
    }

    if (right != nullptr){

        if (depth_of_right - 1 != max(right->depth_of_right, right->depth_of_left)){

            node_error = right_depth;
            return false;
        }

        if (size_of_right_tree - 1 != right->size_of_left_tree + right->size_of_right_tree){
            
            node_error = right_size;
            return false;
        }
    } else{

        if (size_of_right_tree != 0){

            node_error = right_size;
            return false;
        }

        if (depth_of_right != 0){

            node_error = right_depth;
            return false;
        }
    }

    return true;
}

void Node::left_rotation(){ 
    
    Node* cur_prev = prev;
    Node* cur_left = left;

    left = cur_left->right;
    if (left != nullptr){

        left->prev = this;
    }
    
    if (cur_prev != nullptr){

        if (cur_prev->left == this){

            cur_prev->left = cur_left;
            cur_left->prev = cur_prev;
        } else{

            cur_prev->right = cur_left;
            cur_left->prev = cur_prev;
        }
    } else{

        cur_left->prev = nullptr;
    }

    cur_left->right = this;
    prev = cur_left;

    fix_node_data();
    prev->fix_node_data();
}

void Node::right_rotation(){

    //printf("\tright rotation\n");
    Node* cur_prev = prev;
    Node* cur_right = right;

    right = cur_right->left;
    if (right != nullptr){

        right->prev = this;
    }

    if (cur_prev != nullptr){

        if (cur_prev->left == this){

            cur_prev->left = cur_right;
            cur_right->prev = cur_prev;
        } else{

            cur_prev->right = cur_right;
            cur_right->prev = cur_prev;
        }
    } else{

        cur_right->prev = nullptr;
    }

    cur_right->left = this;
    prev = cur_right;

    fix_node_data();
    prev->fix_node_data();
}

void Node::big_left_rotation(){

    left->right_rotation();
    left_rotation();
}

void Node::big_right_rotation(){

    right->left_rotation();
    right_rotation();
}

void Node::fix_node_data(){

    depth_of_left = 0;
    depth_of_right = 0;
    size_of_left_tree = 0;
    size_of_right_tree = 0;

    if (left != nullptr){

        depth_of_left = max(left->depth_of_left, left->depth_of_right) + 1;
        size_of_left_tree = left->size_of_left_tree + left->size_of_right_tree + 1;
    }

    if (right != nullptr){

        depth_of_right = max(right->depth_of_left, right->depth_of_right) + 1;
        size_of_right_tree = right->size_of_left_tree + right->size_of_right_tree + 1;
    }
}

void Node::add_left_node(T_key new_key){

    assert(left == nullptr);

    depth_of_left = 1;
    size_of_left_tree = 1;

    left = new Node(new_key, this);
}

void Node::add_right_node(T_key new_key){

    assert(right == nullptr);

    depth_of_right = 1;
    size_of_right_tree = 1;

    right = new Node(new_key, this);
}


AVL_tree::AVL_tree(){

    root = nullptr;

    number_of_rotations = 0;
}

AVL_tree::~AVL_tree(){

    Node* cur_node = root;

    while ((cur_node->go_left() != nullptr) || (cur_node->go_left() != nullptr) || (cur_node->go_back() != nullptr)){

        if (cur_node->go_left() != nullptr){

            cur_node = cur_node->go_left();
        } else{

            if (cur_node->go_right() != nullptr){

                cur_node = cur_node->go_right();
            } else{

                if (cur_node->go_back()->go_right() == cur_node){

                    cur_node = cur_node->go_back();
                    cur_node->delete_right();
                } else{
                    
                    cur_node = cur_node->go_back();
                    cur_node->delete_left();
                }
            }
        }
    }

    root->~Node();
}

long AVL_tree::add_new_elem(T_key new_key){

    if (root == nullptr){

        root = new Node(new_key);

        return 0;
    }

    Node* cur_node = root;
    bool elem_has_been_added = false;
    long number_of_rot_before = number_of_rotations;

    while (!elem_has_been_added){
        
        if (new_key < cur_node->get_key()){
            
            cur_node->increase_size_of_left();

            if (cur_node->go_left() == nullptr){

                cur_node->add_left_node(new_key);
                balance_tree(cur_node->go_left());
                elem_has_been_added = true;
            } else{

                cur_node = cur_node->go_left();
            }
        } else{
            
            cur_node->increase_size_of_right();

            if (cur_node->go_right() == nullptr){

                cur_node->add_right_node(new_key);
                balance_tree(cur_node->go_right());
                elem_has_been_added = true;
            } else{

                cur_node = cur_node->go_right();
            }
        }

        assert(cur_node != nullptr);
    }

    if (root->go_back() != nullptr){

        root = root->go_back();

        assert(root->go_back() == nullptr);
    }

    assert(root->check_balance() == true);   

    return number_of_rotations - number_of_rot_before;
}

void AVL_tree::balance_tree(Node* cur_tree_root){

    switch (cur_tree_root->choose_rotation()){

    case left_rot:
        cur_tree_root->left_rotation();
        cur_tree_root = cur_tree_root->go_back();
        number_of_rotations++;
        break;

    case right_rot:
        cur_tree_root->right_rotation();
        cur_tree_root = cur_tree_root->go_back();
        number_of_rotations++;
        break;

    case big_left_rot:
        cur_tree_root->big_left_rotation();
        cur_tree_root = cur_tree_root->go_back();
        number_of_rotations += 2;
        break;

    case big_right_rot:
        cur_tree_root->big_right_rotation();
        cur_tree_root = cur_tree_root->go_back();
        number_of_rotations += 2;
        break;

    case none:
        cur_tree_root->fix_node_data();
    }

    if (cur_tree_root->go_back() != nullptr){

        cur_tree_root->go_back()->fix_node_data();
        balance_tree(cur_tree_root->go_back());
    }
}

void print_tree(std::ostream& outp_stream, Node* cur_node){

    if (cur_node != nullptr){

        print_tree(outp_stream, cur_node->go_left());
        cur_node->print_node(outp_stream);
        print_tree(outp_stream, cur_node->go_right());
    }
}

void AVL_tree::dump(std::ostream& outp_stream){

    outp_stream << "root: ";
    root->print_node(outp_stream);
    outp_stream << '\n';
    print_tree(outp_stream, root);
}

bool check_tree(std::ostream& outp_stream, Node* cur_root){

    if (cur_root == nullptr){
        
        return true;
    }

    if (cur_root->check_data() == false){

        outp_stream << "error: [";

        switch (cur_root->get_error()){

        case left_depth:
            outp_stream << "left depth corrupted";
            break;
        
        case left_size:
            outp_stream << "left tree height corrupted";
            break;

        case right_depth:
            outp_stream << "right depth corrupted";
            break;

        case right_size:
            outp_stream << "right tree height corrupted";
            break;
        } 

        outp_stream << "] node: ";

        cur_root->print_node(outp_stream);

        return false;
    } else{

        return (check_tree(outp_stream, cur_root->go_left()) && (check_tree(outp_stream, cur_root->go_right())));
    }   
}

bool AVL_tree::check_context(std::ostream& outp_stream) const{

    if (root->check_balance() == false){

        outp_stream << "Tree is unbalanced\n";
        return false;
    }
    return check_tree(outp_stream, root);
}

