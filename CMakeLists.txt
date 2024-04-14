cmake_minimum_required(VERSION 3.5) # check CMake ver

include(libbuilder.cmake)

project(Stud CXX)# set name of project

#set Release/Debug dir place
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "..")

set(SOURCEDIR ${PROJECT_SOURCE_DIR}/src)

set(SRCS ${SOURCEDIR}/main.cpp;
         ${SOURCEDIR}/lr1.cpp;
         ${SOURCEDIR}/lr2.cpp;
         ${SOURCEDIR}/lr3.cpp;)

add_executable(Stud ${SRCS})# main .exe file
include_directories(${SOURCEDIR}/include)# global include dirs

set_property(TARGET Stud PROPERTY CXX_STANDARD 17)

find_package(Threads REQUIRED)
find_package(OpenMP REQUIRED)

target_link_libraries(Stud PUBLIC pthread OpenMP::OpenMP_CXX)


