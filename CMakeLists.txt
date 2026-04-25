cmake_minimum_required(VERSION 3.10)
project(SteganoPi)

# Setăm standardul C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Căutăm pachetul OpenCV
find_package(OpenCV REQUIRED)

# Includem directoarele OpenCV
include_directories(${OpenCV_INCLUDE_DIRS})

# Definim executabilul (înlocuiește main.cpp cu numele fișierului tău dacă e diferit)
add_executable(SteganoPi main.cpp)

# Legăm bibliotecile OpenCV de executabil
target_link_libraries(SteganoPi ${OpenCV_LIBS})