cmake_minimum_required(VERSION 3.17)
project(ACLIBConverter)


set(CMAKE_CXX_STANDARD 11)

# AC uses python 3.3
# Requires msvc++ 10.0
# find_package(PythonLibs 3.3 EXACT REQUIRED)
find_package(PythonLibs REQUIRED)

include_directories(
        source/
        ../Common/
        ${PYTHON_INCLUDE_DIRS})

set(SOURCES
        ../Common/AC.hpp
        ../Common/ACLIB.hpp

        source/python/ACLIBConverter.cpp
        source/Converter.cpp
        source/Converter.hpp)

# The module
add_library(ACLIBConverter
        ${SOURCES}
        ${PYTHON_LIBRARIES})
target_link_libraries(ACLIBConverter ${PYTHON_LIBRARIES})
