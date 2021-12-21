cmake_minimum_required(VERSION 3.20)
project(Checkers_Engine C)

set(CMAKE_C_STANDARD 99)

add_executable(Checkers_Engine main.c board.h evaluation.h move_logics.h simulation.h engine.h move_tree.h)
