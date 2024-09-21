cmake_minimum_required(VERSION 3.30)
project(graph C)

set(CMAKE_C_STANDARD 11)

add_executable(graph main.c
        datatypes/list.c
        datatypes/list.h
        datatypes/types.h
        parser.c
        parser.h
        datatypes/unit.c
        datatypes/unit.h
        evaluator.c
        evaluator.h)
target_link_libraries(graph PUBLIC m)
