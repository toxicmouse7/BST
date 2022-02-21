cmake_minimum_required(VERSION 3.20)
project(TreeIter)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "/await")

add_executable(TreeIter main.cpp BST.hpp)
