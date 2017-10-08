cmake_minimum_required(VERSION 3.8)
project(nspire_io)

list(APPEND CMAKE_C_FLAGS "-std=c99 -W -Wall -Wno-incompatible-library-redeclaration -Wno-parentheses -Wno-pointer-sign -Wno-incompatible-pointer-types")

include_directories(${CMAKE_SOURCE_DIR}/include
                    ${CMAKE_SOURCE_DIR}/single
                    ~/CEdev/include)

file(GLOB_RECURSE SRC_FILES   src      false *.h *.c)

add_executable(nspire_io ${SRC_FILES})