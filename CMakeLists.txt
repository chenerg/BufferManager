cmake_minimum_required(VERSION 3.8)
project(BufferManager)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp LRU.cpp types.h BMgr.cpp)
add_executable(BufferManager ${SOURCE_FILES})