#define BOOST_TEST_MODULE First_TestSuite
#include <boost/test/included/unit_test.hpp>
#include "headers/AVL_tree.hpp"

#define MAX_LEN 100

char set_byte(char cur_num, int byte_num){

    unsigned char tmp = 1;
    return (cur_num | (tmp << (byte_num - 1)));
}

class AVL_test: public AVL_tree{

    int number_of_elems;

    void print_node(std::ostream& outp_stream, Node* cur_node);

    bool check_node_connections(Node* cur_node) const;

public:

    int test_dump(std::ostream& outp_stream = std::cout);

    bool check_connections() const;

    bool check_context(std::ostream& outp_stream = std::cout) const;
};

void AVL_test::print_node(std::ostream& outp_stream, Node* cur_node){

    if (cur_node != nullptr){

        print_node(outp_stream, cur_node->go_left());
        outp_stream << cur_node->get_key() << '\n';
        number_of_elems++;
        print_node(outp_stream, cur_node->go_right());
    }
}

int AVL_test::test_dump(std::ostream& outp_stream){

    number_of_elems = 0;
    print_node(outp_stream, root);

    return number_of_elems;
}

bool AVL_test::check_node_connections(Node* cur_node) const{

    if (cur_node == nullptr){

        return true;
    }

    if (cur_node->go_back() != nullptr){

        if ((cur_node->go_back()->go_left() != cur_node) && (cur_node->go_back()->go_right() != cur_node)){

            return false;
        }
    } else{

        return false;
    }

    return (check_node_connections(cur_node->go_left()) && (check_node_connections(cur_node->go_right())));
}

bool AVL_test::check_connections() const{

    if (root == nullptr){

        return false;
    }

    if (root->go_back() != nullptr){

        return false;
    }
    
    return (check_node_connections(root->go_left()) && (check_node_connections(root->go_right())));
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

bool AVL_test::check_context(std::ostream& outp_stream) const{

    if (root->check_balance() == false){

        outp_stream << "Tree is unbalanced\n";
        return false;
    }
    return check_tree(outp_stream, root);
}


enum Test_errors{

    context_error = 1,
    balance_error = 2,
    data_error = 3,
    number_of_elems_error = 4,
    connection_error = 5,
    get_last_elem_error = 6,
    min_number_error = 7
};

BOOST_AUTO_TEST_SUITE(AVL_tree_tests)

BOOST_AUTO_TEST_CASE(First){

    int file_size = 1000;

    char error = 0;
    AVL_test test_tree;
    std::ofstream outp_file("../bin/first_out.txt", std::ios_base::trunc);
    std::ifstream input_file("../bin/first_test.txt");
    T_key elem = 0, prev_elem = 0;
    int number_of_elems = 0;

    for (int i = 0; i < file_size; i++){

        input_file >> elem;
        test_tree.add_new_elem(elem);
    }

    if (test_tree.check_context(outp_file) == false){

        error = set_byte(error, context_error);
    }

    number_of_elems = test_tree.test_dump(outp_file);
    outp_file.close();
    input_file.close();

    input_file.open("first_out.txt", std::ios_base::in);
    input_file >> prev_elem;

    for (int i = 0; i < number_of_elems - 1; i++){

        input_file >> elem;

        if (prev_elem >= elem){

            error = set_byte(error, data_error);
        }
    }

    if (number_of_elems != file_size){

        error = set_byte(error, number_of_elems_error);
    }

    if (test_tree.check_connections() == false){
        
        error = set_byte(error, connection_error);
    }

    BOOST_REQUIRE_EQUAL(error, 0);
}

BOOST_AUTO_TEST_CASE(Second){

    int file_size = 10000;

    char error = 0;
    AVL_test test_tree;
    std::ofstream outp_file("../bin/second_out.txt", std::ios_base::trunc);
    std::ifstream input_file("../bin/second_test.txt");
    T_key elem = 0, prev_elem = 0;
    int number_of_elems = 0;

    for (int i = 0; i < file_size; i++){

        input_file >> elem;
        test_tree.add_new_elem(elem);
    }

    if (test_tree.check_context(outp_file) == false){

        error = set_byte(error, context_error);
    }

    number_of_elems = test_tree.test_dump(outp_file);
    outp_file.close();
    input_file.close();

    input_file.open("second_out.txt", std::ios_base::in);
    input_file >> prev_elem;

    for (int i = 0; i < number_of_elems - 1; i++){

        input_file >> elem;

        if (prev_elem >= elem){

            error = set_byte(error, data_error);
        }
    }

    if (number_of_elems != file_size){

        error = set_byte(error, number_of_elems_error);
    }

    if (test_tree.check_connections() == false){
        
        error = set_byte(error, connection_error);
    }

    BOOST_REQUIRE_EQUAL(error, 0);
}

BOOST_AUTO_TEST_CASE(Third){

    int file_size = 99995;

    char error = 0;
    AVL_test test_tree;
    std::ofstream outp_file("../bin/third_out.txt", std::ios_base::trunc);
    std::ifstream input_file("../bin/third_test.txt");
    T_key elem = 0, prev_elem = 0;
    int number_of_elems = 0;

    for (int i = 0; i < file_size; i++){

        input_file >> elem;
        test_tree.add_new_elem(elem);
    }

    if (test_tree.check_context(outp_file) == false){

        error = set_byte(error, context_error);
    }

    number_of_elems = test_tree.test_dump(outp_file);
    outp_file.close();
    input_file.close();

    input_file.open("third_out.txt", std::ios_base::in);
    input_file >> prev_elem;

    for (int i = 0; i < number_of_elems - 1; i++){

        input_file >> elem;

        if (prev_elem >= elem){

            error = set_byte(error, data_error);
        }
    }

    if (number_of_elems != file_size){

        error = set_byte(error, number_of_elems_error);
    }

    if (test_tree.check_connections() == false){
        
        error = set_byte(error, connection_error);
    }

    BOOST_REQUIRE_EQUAL(error, 0);
}

BOOST_AUTO_TEST_CASE(Fourth){

    int file_size = 999957;

    char error = 0;
    AVL_test test_tree;
    std::ofstream outp_file("../bin/fourth_out.txt", std::ios_base::trunc);
    std::ifstream input_file("../bin/fourth_test.txt");
    T_key elem = 0, prev_elem = 0;
    int number_of_elems = 0;

    for (int i = 0; i < file_size; i++){

        input_file >> elem;
        test_tree.add_new_elem(elem);
    }

    if (test_tree.check_context(outp_file) == false){

        error = set_byte(error, context_error);
    }

    number_of_elems = test_tree.test_dump(outp_file);
    outp_file.close();
    input_file.close();

    input_file.open("fourth_out.txt", std::ios_base::in);
    input_file >> prev_elem;

    for (int i = 0; i < number_of_elems - 1; i++){

        input_file >> elem;

        if (prev_elem >= elem){

            error = set_byte(error, data_error);
        }
    }

    if (number_of_elems != file_size){

        error = set_byte(error, number_of_elems_error);
    }

    if (test_tree.check_connections() == false){
        
        error = set_byte(error, connection_error);
    }

    BOOST_REQUIRE_EQUAL(error, 0);
}

BOOST_AUTO_TEST_CASE(Fifth){

    int file_size = 500000;

    char error = 0;
    AVL_test test_tree;
    std::ofstream outp_file("../bin/fifth_out.txt", std::ios_base::trunc);
    std::ifstream input_file("../bin/fifth_test.txt");
    T_key elem = 0, prev_elem = 0;
    int number_of_elems = 0;

    for (int i = 0; i < file_size; i++){

        input_file >> elem;
        test_tree.add_new_elem(elem);
    }

    if (test_tree.check_context(outp_file) == false){

        error = set_byte(error, context_error);
    }

    number_of_elems = test_tree.test_dump(outp_file);
    outp_file.close();
    input_file.close();

    input_file.open("fifth_out.txt", std::ios_base::in);
    input_file >> prev_elem;

    for (int i = 0; i < number_of_elems - 1; i++){

        input_file >> elem;

        if (prev_elem >= elem){

            error = set_byte(error, data_error);
        }
    }

    if (number_of_elems != file_size){

        error = set_byte(error, number_of_elems_error);
    }

    if (test_tree.check_connections() == false){
        
        error = set_byte(error, connection_error);
    }

    BOOST_REQUIRE_EQUAL(error, 0);
}

BOOST_AUTO_TEST_CASE(Almost_eternity){

    int file_size = 5000000;

    char error = 0;
    AVL_test test_tree;
    std::ofstream outp_file("../bin/eternity_out.txt", std::ios_base::trunc);
    std::ifstream input_file("../bin/eternity_test.txt");
    T_key elem = 0, prev_elem = 0;
    int number_of_elems = 0;

    for (int i = 0; i < file_size; i++){

        input_file >> elem;
        test_tree.add_new_elem(elem);
    }

    if (test_tree.check_context(outp_file) == false){

        error = set_byte(error, context_error);
    }

    number_of_elems = test_tree.test_dump(outp_file);
    outp_file.close();
    input_file.close();

    input_file.open("eternity_out.txt", std::ios_base::in);
    input_file >> prev_elem;

    for (int i = 0; i < number_of_elems - 1; i++){

        input_file >> elem;

        if (prev_elem >= elem){

            error = set_byte(error, data_error);
        }
    }

    if (number_of_elems != file_size){

        error = set_byte(error, number_of_elems_error);
    }

    if (test_tree.check_connections() == false){
        
        error = set_byte(error, connection_error);
    }

    BOOST_REQUIRE_EQUAL(error, 0);
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(Search_tests)

BOOST_AUTO_TEST_CASE(First_search){

    int file_size = 1000;

    char error = 0;
    AVL_test test_tree;
    std::ifstream input_file("../bin/first_test.txt");
    T_key elem = 0;
    int number_of_elems_less = 0, min = 0;

    for (int i = 0; i < file_size; i++){

        input_file >> elem;
        test_tree.add_new_elem(elem);
    }

    for (int i = 0; i < file_size; i++){

        min = test_tree.get_last_elem(i + 1);
        number_of_elems_less = test_tree.number_of_elems_less_than(i);

        if (min != i){

            error = set_byte(error, get_last_elem_error);
        }

        if (number_of_elems_less != i){

            error = set_byte(error, min_number_error);
        }
    }

    BOOST_REQUIRE_EQUAL(error, 0);
}

BOOST_AUTO_TEST_CASE(Second_search){

    int file_size = 10000;

    char error = 0;
    AVL_test test_tree;
    std::ifstream input_file("../bin/second_test.txt");
    T_key elem = 0;
    int number_of_elems_less = 0, min = 0;

    for (int i = 0; i < file_size; i++){

        input_file >> elem;
        test_tree.add_new_elem(elem);
    }

    for (int i = 0; i < file_size; i++){

        min = test_tree.get_last_elem(i + 1);
        number_of_elems_less = test_tree.number_of_elems_less_than(i);

        if (min != i){

            error = set_byte(error, get_last_elem_error);
        }

        if (number_of_elems_less != i){

            error = set_byte(error, min_number_error);
        }
    }

    BOOST_REQUIRE_EQUAL(error, 0);
}

BOOST_AUTO_TEST_CASE(Fifth_search){

    int file_size = 500000;

    char error = 0;
    AVL_test test_tree;
    std::ifstream input_file("../bin/fifth_test.txt");
    T_key elem = 0;
    int number_of_elems_less = 0, min = 0;

    for (int i = 0; i < file_size; i++){

        input_file >> elem;
        test_tree.add_new_elem(elem);
    }

    for (int i = 0; i < file_size; i++){

        min = test_tree.get_last_elem(i + 1);
        number_of_elems_less = test_tree.number_of_elems_less_than(i);

        if (min != i){

            error = set_byte(error, get_last_elem_error);
        }

        if (number_of_elems_less != i){

            error = set_byte(error, min_number_error);
        }
    }

    BOOST_REQUIRE_EQUAL(error, 0);
}

BOOST_AUTO_TEST_CASE(Eternity_search){

    int file_size = 5000000;

    char error = 0;
    AVL_test test_tree;
    std::ifstream input_file("../bin/eternity_test.txt");
    T_key elem = 0;
    int number_of_elems_less = 0, min = 0;

    for (int i = 0; i < file_size; i++){

        input_file >> elem;
        test_tree.add_new_elem(elem);
    }

    for (int i = 0; i < file_size; i++){

        min = test_tree.get_last_elem(i + 1);
        number_of_elems_less = test_tree.number_of_elems_less_than(i);

        if (min != i){

            error = set_byte(error, get_last_elem_error);
        }

        if (number_of_elems_less != i){

            error = set_byte(error, min_number_error);
        }
    }

    BOOST_REQUIRE_EQUAL(error, 0);
}

BOOST_AUTO_TEST_SUITE_END()