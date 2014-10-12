#set(Boost_USE_STATIC_LIBS   ON)
set(Boost_USE_MULTITHREADED ON)
find_package( Boost 1.41.0
              REQUIRED
              COMPONENTS program_options )
if (NOT Boost_FOUND)
  MESSAGE(FATAL_ERROR, "Unable to find Boost")
ELSE (NOT Boost_FOUND)
  MESSAGE("Boost headers: " ${Boost_INCLUDE_DIRS})
  MESSAGE("Boost libs:    " ${Boost_LIBRARIES})
ENDIF(NOT Boost_FOUND)

# Include the directory itself as a path to include directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

# For a large number of source files you can create it in a simpler way
# using file() function:
file(GLOB cli_SOURCES *.cpp)

# Create an executable file called cli from sources:
add_executable(cli ${cli_SOURCES})
target_link_libraries(cli ${Boost_LIBRARIES} )

INSTALL(TARGETS cli
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)

