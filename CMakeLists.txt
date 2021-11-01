cmake_minimum_required(VERSION 2.8)
find_package(Boost 1.40 COMPONENTS program_options REQUIRED)

add_executable(Search_tree src/main.cpp src/headers/AVL_tree.hpp src/AVL_tree.cpp)

add_executable(Search_tree_test src/test.cpp src/headers/AVL_tree.hpp src/AVL_tree.cpp )

target_link_libraries(Search_tree_test Boost::program_options)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)