cmake_minimum_required(VERSION 3.6)
project(glad)

add_library(glad glad.h glad.c)
target_include_directories(glad PUBLIC ./)

