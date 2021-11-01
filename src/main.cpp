#include "headers/AVL_tree.hpp"

int main(){

    T_key elem = 0, outp_elem = 0;
    char key = '!';
    AVL_tree new_tree;

    while(!std::cin.eof()){

        key = '!';
        std::cin >> key;
        std::cin >> elem;
        
        switch (key){

            case 'k':
                new_tree.add_new_elem(elem);
                new_tree.check_error();
                break;
            case 'm':
                outp_elem = new_tree.get_last_elem(elem);

                if (new_tree.check_error()){
                    std::cout << outp_elem << ' ';
                } else{
                    std::cout << "nan ";
                }
                break;
            
            case 'n':
                std::cout << new_tree.number_of_elems_less_than(elem) << ' ';
                break;

            default:
                std::cerr << "Error: Undefined command [" << key << "]\n";
                break; 
        }
    }
    std::cout << '\n';
}