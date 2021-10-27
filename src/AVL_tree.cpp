#include "headers/AVL_tree.hpp"

typedef struct Coordinates_struct{

    int x;
    int y;
} Coordinates;

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

bool go_to_node(int x, Node*& cur_node, Coordinates& cur_node_coord, int vertical_size){

    bool node_has_found = false;

    if (cur_node_coord.x == x){

        node_has_found = true;
        return true;
    }

    while (!node_has_found){

        if (x < cur_node_coord.x){

            if (cur_node_coord.x - x >= pow(2, vertical_size - cur_node_coord.y - 1)){

                if (cur_node->go_back() == nullptr){

                    return false;
                }

                if (cur_node->go_back()->go_left() == cur_node){

                    cur_node_coord.x -= pow(2, vertical_size - cur_node_coord.y - 1);
                } else{

                    cur_node_coord.x += pow(2, vertical_size - cur_node_coord.y - 1);
                }

                cur_node = cur_node->go_back();
                cur_node_coord.y--;
            } else{

                if (cur_node->go_right() == nullptr){

                    return false;
                }
                cur_node = cur_node->go_right();
                cur_node_coord.x -= pow(2, vertical_size - cur_node_coord.y - 2);
                cur_node_coord.y++;
            }
        } else{

            if (x - cur_node_coord.x >= pow(2, vertical_size - cur_node_coord.y - 1)){

                if (cur_node->go_back() == nullptr){

                    return false;
                }

                if (cur_node->go_back()->go_left() == cur_node){

                    cur_node_coord.x -= pow(2, vertical_size - cur_node_coord.y - 1);
                } else{

                    cur_node_coord.x += pow(2, vertical_size - cur_node_coord.y - 1);
                }
                cur_node = cur_node->go_back();
                cur_node_coord.y--;
            } else{

                if (cur_node->go_left() == nullptr){

                    return false;
                }
                cur_node = cur_node->go_left();
                cur_node_coord.x += pow(2, vertical_size - cur_node_coord.y - 2);
                cur_node_coord.y++;
            }
        }

        if (cur_node_coord.x == x){

            node_has_found = true;
        }
    }

    return true;
}

AVL_tree::AVL_tree(const AVL_tree& old_tree){

    root = nullptr;
    number_of_rotations = 0;

    Node* cur_node = old_tree.root;
    Coordinates cur_node_coord{0, 0};
    int  vertical_tree_size = max(old_tree.root->get_left_depth(), old_tree.root->get_right_depth()) + 1, horizontal_tree_size = pow(2, vertical_tree_size) - 1, depth = 0;

    if (old_tree.root != nullptr){

        while(cur_node->go_right() != nullptr){

            cur_node = cur_node->go_right();
            depth++;
        }
        cur_node_coord.y = depth;

        for (int x = 0; x < horizontal_tree_size; x++){

            if (go_to_node(x, cur_node, cur_node_coord, vertical_tree_size)){

                add_new_elem(cur_node->get_key());
            }
        }
    }
}

AVL_tree::~AVL_tree(){

    Node* cur_node = root;

    if (root != nullptr){

        while ((cur_node->go_left() != nullptr) || (cur_node->go_right() != nullptr) || (cur_node->go_back() != nullptr)){

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

void AVL_tree::dump(std::ostream& outp_stream) const{

    outp_stream << "root: ";
    root->print_node(outp_stream);
    outp_stream << "number of rotations: " << number_of_rotations << '\n';
    print_tree(outp_stream, root);
}

Node* AVL_tree::find_elems_node(T_key elem) const{

    Node* cur_node = root;

    while (cur_node != nullptr){

        if (cur_node->get_key() == elem){

            return cur_node;
        }

        if (elem < cur_node->get_key()){

            cur_node = cur_node->go_left();
        } else{

            cur_node = cur_node->go_right();
        }
    }

    return nullptr;
}

bool AVL_tree::find_elem(T_key elem) const{
    
    if (find_elems_node(elem) != nullptr){

        return true;
    } else{

        return false;
    }
}

int AVL_tree::number_of_elems_less_than(int cur_elem) const{

    Node* cur_node = root;
    int elems_less_then = 0;

    while (cur_node != nullptr){

        if (cur_node->get_key() == cur_elem){

            return elems_less_then + cur_node->get_left_tree_size();
        }

        if (cur_elem < cur_node->get_key()){

            cur_node = cur_node->go_left();
        } else{

            elems_less_then += 1 + cur_node->get_left_tree_size();
            cur_node = cur_node->go_right();
        }
    }

    return elems_less_then;
}

T_key AVL_tree::get_last_elem(int degree_of_last_elem) const{

    bool elem_has_found = false;
    int elems_less_than_cur_node = 0;
    Node* cur_node = root;

    if (root == nullptr){

        return -1;
    }

    if (degree_of_last_elem < 1){

        return -1;
    }

    if (degree_of_last_elem > root->get_left_tree_size() + root->get_right_tree_size() + 1){

        return -1;
    }

    while (!elem_has_found){

        if (cur_node == nullptr){

            return -1;
        }

        if (degree_of_last_elem - 1 == elems_less_than_cur_node + cur_node->get_left_tree_size()){

            elem_has_found = true;
        } else{

            if (degree_of_last_elem - 1 < elems_less_than_cur_node + cur_node->get_left_tree_size()){

                cur_node = cur_node->go_left();
                assert(cur_node != nullptr);
            } else{

                elems_less_than_cur_node += 1 + cur_node->get_left_tree_size();
                cur_node = cur_node->go_right();
                assert(cur_node != nullptr);
            }
        }
    }
    
    return cur_node->get_key();
}