#include "headers/AVL_tree.hpp"

int main(){

    int number_of_itter = 0;
    T_key elem = 0;
    AVL_tree new_tree;

    std::cin >> number_of_itter;

    for (int i = 0; i < number_of_itter; i++){

        std::cin >> elem;
        
        new_tree.add_new_elem(elem);
    }

    new_tree.dump();
    std::cout << "tree check: " << new_tree.check_context() << '\n';
}