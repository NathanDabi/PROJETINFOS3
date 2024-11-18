cmake_minimum_required(VERSION 3.26)
project(Ford_Fulkerson C)

set(CMAKE_C_STANDARD 11)

add_executable(Ford_Fulkerson main.c
        Graphe.h)
