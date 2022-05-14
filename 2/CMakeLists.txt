cmake_minimum_required(VERSION 3.21)
project(CapHH)

set(CMAKE_CXX_STANDARD 14)

add_executable(CapHH main.cpp Saida.cpp Saida.h Configures.cpp Configures.h Grammar.cpp Grammar.h Parser.cpp Parser.h Search.cpp Search.h Subject.cpp Subject.h Tree.cpp Tree.h No.cpp No.h Instances.cpp Instances.h)
