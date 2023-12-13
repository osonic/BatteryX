cmake_minimum_required(VERSION 3.2)
project(WaveAnalyzer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES main.cpp)
add_executable(WaveAnalyzer ${SOURCE_FILES} dfft.h dfft.cpp dfft.cpp dfft.h Render.cpp Render.h wave.cpp wave.h)
