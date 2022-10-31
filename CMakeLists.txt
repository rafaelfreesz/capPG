cmake_minimum_required(VERSION 3.21)
project(CapHH)

set(CMAKE_CXX_STANDARD 14)

add_executable(CapHH main.cpp Saida.cpp Saida.h Configures.h Grammar.cpp Grammar.h Search.cpp Search.h Individuo.cpp Individuo.h Tree.cpp Tree.h Node.cpp Node.h Instance.cpp Instance.h Arena.cpp Arena.h)
