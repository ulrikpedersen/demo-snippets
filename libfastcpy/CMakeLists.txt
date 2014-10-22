file( GLOB fastcpy_SOURCES *.cpp *.c )

add_library( fastcpy ${fastcpy_SOURCES} )
include_directories (fastcpy ${CMAKE_CURRENT_SOURCE_DIR})
