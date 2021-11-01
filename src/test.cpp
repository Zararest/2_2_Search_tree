#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define BOOST_TEST_MODULE First_TestSuite
#include <boost/test/included/unit_test.hpp>
#include "headers/AVL_tree.hpp"

#define CLASS_TESTS_PATH  "../bin/class_tests/"
#define INPUT_TESTS_PATH  "../bin/input_tests/"
#define MANUAL_TESTS_PATH "../bin/manual_tests/"
#define MAXLEN 100

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


bool cmp_numbers_in_files(std::ifstream& fir_file, std::ifstream& sec_file){

    int fir_elem = 0, sec_elem = 0;

    while ((!fir_file.eof()) && (!sec_file.eof())){

        fir_file >> fir_elem;
        sec_file >> sec_elem;

        if (fir_elem != sec_elem){

            return false;
        }    
    }

    return true;
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

void AVL_tree_tests_test_body(const char* file_name, int file_size){

    std::string otp_file_name(CLASS_TESTS_PATH);
    otp_file_name.append(file_name);
    otp_file_name.append("_out.txt");

    std::string inp_file_name(CLASS_TESTS_PATH);
    inp_file_name.append(file_name);
    inp_file_name.append("_test.txt");

    char error = 0;
    AVL_test test_tree;
    std::ifstream input_file(inp_file_name); 
    std::ofstream outp_file(otp_file_name, std::ios_base::trunc);
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

    input_file.open(otp_file_name, std::ios_base::in);
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

BOOST_AUTO_TEST_CASE(First){

    AVL_tree_tests_test_body("first", 1000);
}

BOOST_AUTO_TEST_CASE(Second){

    AVL_tree_tests_test_body("second", 10000);
}

BOOST_AUTO_TEST_CASE(Third){

    AVL_tree_tests_test_body("third", 99995);
}

BOOST_AUTO_TEST_CASE(Fourth){

    AVL_tree_tests_test_body("fourth", 999957);
}

BOOST_AUTO_TEST_CASE(Fifth){

    AVL_tree_tests_test_body("fifth", 500000);
}

BOOST_AUTO_TEST_CASE(Almost_eternity){

    AVL_tree_tests_test_body("eternity", 5000000);
}

BOOST_AUTO_TEST_CASE(Copy){

    int file_size = 500000;

    char error = 0;
    AVL_test test_tree;
    std::ofstream outp_file("../bin/class_tests/copy_out.txt", std::ios_base::trunc);
    std::ifstream input_file("../bin/class_tests/fifth_test.txt");
    T_key elem = 0, prev_elem = 0;
    int number_of_elems = 0;

    for (int i = 0; i < file_size; i++){

        input_file >> elem;
        test_tree.add_new_elem(elem);
    }

    AVL_test test_copied_tree(test_tree);
    number_of_elems = test_copied_tree.test_dump(outp_file);
    outp_file.close();
    input_file.close();

    input_file.open("../bin/class_tests/copy_out.txt", std::ios_base::in);
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

    if (test_copied_tree.check_connections() == false){
        
        error = set_byte(error, connection_error);
    }

    BOOST_REQUIRE_EQUAL(error, 0);
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(Search_tests)

void AVL_search_tests_test_body(const char* file_name, int file_size){

    std::string inp_file_name(CLASS_TESTS_PATH);
    inp_file_name.append(file_name);
    inp_file_name.append("_test.txt");

    char error = 0;
    AVL_test test_tree;
    std::ifstream input_file(inp_file_name);
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

BOOST_AUTO_TEST_CASE(First_search){

    AVL_search_tests_test_body("first", 1000);
}

BOOST_AUTO_TEST_CASE(Second_search){

    AVL_search_tests_test_body("second", 10000);
}

BOOST_AUTO_TEST_CASE(Fifth_search){

    AVL_search_tests_test_body("fifth", 500000);
}

BOOST_AUTO_TEST_CASE(Eternity_search){

    int file_size = 5000000;

    AVL_search_tests_test_body("eternity", 5000000);
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(Programm_tests)

void AVL_programm_tests_test_body(const char* file_name){

    std::string data_file_name(INPUT_TESTS_PATH);
    data_file_name.append(file_name);
    data_file_name.append("_test.txt");

    std::string prog_out_file_name(INPUT_TESTS_PATH);
    prog_out_file_name.append(file_name);
    prog_out_file_name.append("_test_prog_out.txt");

    std::string real_out_file_name(INPUT_TESTS_PATH);
    real_out_file_name.append(file_name);
    real_out_file_name.append("_test_out.txt");

    T_key elem = 0;
    char key = '!';
    AVL_tree new_tree;
    std::ifstream data(data_file_name);
    std::ofstream prog_out(prog_out_file_name);

    while(!data.eof()){

        key = '!';
        data >> key;
        data >> elem;
        
        switch (key){

        case 'k':
            new_tree.add_new_elem(elem);
            break;
        case 'm':
            prog_out << new_tree.get_last_elem(elem) << ' ';
            break;
        
        case 'n':
            prog_out << new_tree.number_of_elems_less_than(elem) << ' ';
            break;
        }
    }
    prog_out << '\n';
    prog_out.close();

    std::ifstream prog_outp(prog_out_file_name), real_outp(real_out_file_name);

    BOOST_REQUIRE_EQUAL(cmp_numbers_in_files(prog_outp, real_outp), true);
}

BOOST_AUTO_TEST_CASE(Program_test_first){
    //400 элементов в дереве
    AVL_programm_tests_test_body("first");
}

BOOST_AUTO_TEST_CASE(Program_test_second){
    //100000 элементов в дереве
    AVL_programm_tests_test_body("second");
}

BOOST_AUTO_TEST_CASE(Program_test_third){
    //500000 элементов в дереве 
    AVL_programm_tests_test_body("third");
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(Manual_tests)

bool cmp_files(FILE* fir_file, FILE* sec_file){

    char fir_str[MAXLEN], sec_str[MAXLEN];
    int readed = fscanf(fir_file, "%s", fir_str);
    fscanf(sec_file, "%s", sec_str);

    while (readed != EOF){

        if (strcmp(fir_str, sec_str) != 0){
            return false;
        }
        readed = fscanf(fir_file, "%s", fir_str);
        fscanf(sec_file, "%s", sec_str);
    }

    return true;
}

BOOST_AUTO_TEST_CASE(Manual_test_first){

    struct stat buf;
    bool files_is_equal = false;
    if (stat("../build/Search_tree", &buf) != -1){

        system("touch ../bin/manual_tests/first_test_out.txt");
        system("touch ../bin/manual_tests/first_test_errors.txt");
        system("../build/Search_tree < ../bin/manual_tests/first_test.txt > ../bin/manual_tests/first_test_out.txt 2> ../bin/manual_tests/first_test_errors.txt");
        
        FILE* prog_outp = fopen("../bin/manual_tests/first_test_out.txt", "r");
        FILE* real_outp = fopen("../bin/manual_tests/first_test_real_out.txt", "r");
        files_is_equal = cmp_files(prog_outp, real_outp);
        BOOST_REQUIRE_EQUAL(files_is_equal ,true);
    } else{

        std::cerr << "Warning: First manual test cannot be run due to the fact that the main program is not built\n";
    }
    if (files_is_equal){

        unlink("../bin/manual_tests/first_test_out.txt");
        unlink("../bin/manual_tests/first_test_errors.txt");        
    }
}

BOOST_AUTO_TEST_CASE(Manual_test_second){

    struct stat buf;
    bool files_is_equal = false;
    if (stat("../build/Search_tree", &buf) != -1){

        system("touch ../bin/manual_tests/second_test_out.txt");
        system("touch ../bin/manual_tests/second_test_errors.txt");
        system("../build/Search_tree < ../bin/manual_tests/second_test.txt > ../bin/manual_tests/second_test_out.txt 2> ../bin/manual_tests/second_test_errors.txt");
        
        FILE* prog_outp = fopen("../bin/manual_tests/second_test_out.txt", "r");
        FILE* real_outp = fopen("../bin/manual_tests/second_test_real_out.txt", "r");
        files_is_equal = cmp_files(prog_outp, real_outp);
        BOOST_REQUIRE_EQUAL(files_is_equal ,true);
    } else{

        std::cerr << "Warning: Second manual test cannot be run due to the fact that the main program is not built\n";
    }
    if (files_is_equal){

        unlink("../bin/manual_tests/second_test_out.txt");
        unlink("../bin/manual_tests/second_test_errors.txt");        
    }
}

BOOST_AUTO_TEST_CASE(Manual_test_third){

    struct stat buf;
    bool files_is_equal = false;
    if (stat("../build/Search_tree", &buf) != -1){

        system("touch ../bin/manual_tests/third_test_out.txt");
        system("touch ../bin/manual_tests/third_test_errors.txt");
        system("../build/Search_tree < ../bin/manual_tests/third_test.txt > ../bin/manual_tests/third_test_out.txt 2> ../bin/manual_tests/third_test_errors.txt");
        
        FILE* prog_outp = fopen("../bin/manual_tests/third_test_out.txt", "r");
        FILE* real_outp = fopen("../bin/manual_tests/third_test_real_out.txt", "r");
        files_is_equal = cmp_files(prog_outp, real_outp);
        BOOST_REQUIRE_EQUAL(files_is_equal ,true);
    } else{

        std::cerr << "Warning: Third manual test cannot be run due to the fact that the main program is not built\n";
    }
    if (files_is_equal){

        unlink("../bin/manual_tests/third_test_out.txt");
        unlink("../bin/manual_tests/third_test_errors.txt");        
    }
}

BOOST_AUTO_TEST_SUITE_END()