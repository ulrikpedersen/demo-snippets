project(demo-snippets)

cmake_minimum_required(VERSION 2.8)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native -mtune=native -msse2" )
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=native -mtune=native -msse2")

# Appends the cmake/modules path inside the MAKE_MODULE_PATH variable which stores the
# directories of additional CMake modules (ie. MacroOutOfSourceBuild.cmake):
set(CMAKE_MODULE_PATH ${demo-snippets_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

#add_subdirectory(libsrc)

add_subdirectory(libfastcpy)
set(fastcpy_DIR ${CMAKE_SOURCE_DIR}/libfastcpy)

add_subdirectory(clisrc)

