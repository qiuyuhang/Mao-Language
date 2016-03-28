cmake_minimum_required(VERSION 3.3)
project(mao_language)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.c error.h  variable_list.h operand_stack.h operator_stack.h public.h calc.h)
add_executable(mao_language ${SOURCE_FILES})