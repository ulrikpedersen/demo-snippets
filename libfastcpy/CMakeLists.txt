file( GLOB fastcpy_SOURCES *.cpp *.c )

add_library( fastcpy ${fastcpy_SOURCES} )

find_library( ASM_LIB
              libaelf64.a
              ${CMAKE_SOURCE_DIR}
              NO_DEFAULT_PATH )
message("Lib ASM: ${ASM_LIB}")
target_link_libraries(fastcpy ${ASM_LIB} )

include_directories (fastcpy ${CMAKE_CURRENT_SOURCE_DIR})
