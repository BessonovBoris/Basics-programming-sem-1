cmake_minimum_required(VERSION 3.23)
project(Conway_s_Game_Life C)

set(CMAKE_C_STANDARD 99)

add_executable(Conway_s_Game_Life main.c GameLife.h BMP.h BMP.c GameLife.c)
