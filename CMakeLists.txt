cmake_minimum_required(VERSION 3.10)

project(TispLang VERSION 0.1.0 LANGUAGES CXX)

# NOTE: for clangd config
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(USE_DEBUG_MODE True)

if (USE_DEBUG_MODE)
    add_compile_options(-Wall -Wextra -Wpedantic -Werror -g -Og)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror -O2)
endif()

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_HOME_DIRECTORY}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_HOME_DIRECTORY}/build")

include_directories("${CMAKE_HOME_DIRECTORY}/include")

# enable_testing()
add_subdirectory(src)
# add_subdirectory(tests)
