cmake_minimum_required(VERSION 3.0) # sets cmake version
# set(CMAKE_CXX_COMPILER "C:/MinGW/bin/g++.exe" )
# set(CMAKE_C_COMPILER "C:/MinGW/bin/gcc.exe" )
project(IPC_project VERSION 1.0) # creates project

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-std=c11 -pthread -g -w -fopenmp") # windows
# set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-pthread -g -w -fopenmp") # linux


# flag di compilazione -pthread lib per il threading -O0 disabilita ottimizzazione
# optimization levels:
# O0 -> no optimization, faster compilation time
# O1 -> basic optimization, reduces code size and improves performance, no significant change in compilation time [removes dead code, simple inlining, basic loop transformations]
# O2 -> advanced optimization, balances performance improvements with code size [O1 optimizations + loop unrolling, vectorization, better instruction scheduling]
# O3 -> maximum optimization, increased compile time and binary size [O2 optimizations + aggressive inlining, aggressive loop optimizations, function cloning]
# Ofast -> [O3 optimizations + aggressive math optimizations]

set(SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src)
add_compile_definitions("LOG_DEBUG=1")

find_package(MPI REQUIRED)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${MPI_INCLUDE_PATH}
)

add_library(${PROJECT_NAME} STATIC ${SOURCE_DIR}/main.c)
add_library(utils_lib STATIC ${SOURCE_DIR}/utils.c)
add_library(test_lib STATIC ${SOURCE_DIR}/test.c)
add_library(matrix_lib STATIC ${SOURCE_DIR}/matrix_operations.c)
target_link_libraries(matrix_lib PUBLIC utils_lib ${MPI_LIBRARIES}) # ${MPI_LIBRARIES}) # linked utils_lib to matrix_lib, PUBLIC -> if linked to matrix_lib, also links utils_lib
target_link_libraries(test_lib PUBLIC matrix_lib) # linked utils_lib to matrix_lib, PUBLIC -> if linked to matrix_lib, also links utils_lib

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

add_executable(project ${SOURCE_DIR}/main.c)
target_include_directories(project PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
target_link_libraries(project test_lib ${PROJECT_NAME} ${MPI_LIBRARIES})