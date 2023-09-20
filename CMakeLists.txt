cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

project(Tanks)

# Find libs
find_library(ALLEGRO_LIBRARY NAMES liballeg44.dll.a liballeg.dll.a liballeg.so liballeg.dylib)
find_library(ALLEGRO_PNG_LIBRARY NAMES libloadpng.dll.a libloadpng.so libloadpng.dylib)

# Source code
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)

FetchContent_Declare(
  EnTT
  GIT_REPOSITORY https://github.com/skypjack/entt.git
  GIT_TAG        344e03ac64a1f78424ab1150e2d4778e8df8431d
)
FetchContent_MakeAvailable(EnTT)

# Link Libs
if (MINGW)
  target_link_libraries(${PROJECT_NAME} -lmingw32)
endif (MINGW)

target_link_libraries(${PROJECT_NAME} ${ALLEGRO_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${ALLEGRO_PNG_LIBRARY})
target_link_libraries(${PROJECT_NAME} -static-libgcc -static-libstdc++ EnTT::EnTT)

file(COPY ${CMAKE_CURRENT_LIST_DIR}/assets/ DESTINATION ${CMAKE_BINARY_DIR}/build/assets/)
