#include "headers/AVL_tree.hpp"

int main(){

    T_key elem = 0;
    char key = '!';
    AVL_tree new_tree;

    while(!std::cin.eof()){

        key = '!';
        std::cin >> key;
        std::cin >> elem;
        
        switch (key){

        case 'k':
            new_tree.add_new_elem(elem);
            break;
        case 'm':
            std::cout << new_tree.get_last_elem(elem) << ' ';
            break;
        
        case 'n':
            std::cout << new_tree.number_of_elems_less_than(elem) << ' ';
            break;
        }
    }
    std::cout << '\n';
}