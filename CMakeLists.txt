cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

project (Tanks)

# Find libs
find_library(ALLEGRO_LIBRARY NAMES liballeg44)
find_library(ALLEGRO_PNG_LIBRARY NAMES libloadpng)

find_library(PNG_LIBRARY NAMES libpng16)
find_library(ZLIB_LIBRARY NAMES libz)

# Source code
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)

add_executable (${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)

# Link Libs

target_link_libraries(${PROJECT_NAME} ${ALLEGRO_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${ALLEGRO_PNG_LIBRARY})

target_link_libraries(${PROJECT_NAME} ${ZLIB_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${PNG_LIBRARY})

target_link_libraries(${PROJECT_NAME} -static-libgcc -static-libstdc++)