cmake_minimum_required(VERSION 3.21)
project(CapHH)

set(CMAKE_CXX_STANDARD 14)

add_executable(CapHH main.cpp Output.cpp Output.h Configures.h Grammar.cpp Grammar.h Search.cpp Search.h Individual.cpp Individual.h Tree.cpp Tree.h Node.cpp Node.h Instance.cpp Instance.h Arena.cpp Arena.h)
