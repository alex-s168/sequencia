cmake_minimum_required(VERSION 3.28)
project(sequencia C)

set(CMAKE_C_STANDARD 11)

add_executable(sequencia main.c
        sequencia.h
        array.c
        command.c
        value.c
        utils.c
        exec/operations.h
        exec.c
        exec/filter_chars.c
        exec/filter.c
        exec/axis.c
        exec/make.c
        exec/any.c
        exec/same.c
        exec/bool.c
        exec/stats.c
        exec/transform.c
        exec/map.c
        exec/num.c
        exec/tokens.c
        exec/select.c
        exec/join.c
        exec/split.c
        exec/contains.c
        exec/ranges.c
        exec/noempty.c)
