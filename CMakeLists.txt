#--------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.21.3)
project(KusManager VERSION 1.2)

#--------------------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compile options
if (MSVC_VERSION GREATER_EQUAL "1900")
    set(OS BILL_WINDOWS)
    add_compile_options("/std:c++latest")
else()
    set(OS LINUS_LINUX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wl,--no-as-needed -ldl")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_DEBUG")
endif()

#  Code include
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake_scripts/cmake_scripts.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/libraries/libraries.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/sources/sources.cmake)

get_libraries(SOURCES_LIST ${CMAKE_CURRENT_SOURCE_DIR})
get_sources(SOURCES_LIST ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(${PROJECT_NAME} ${SOURCES_LIST})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libraries)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/sources)
add_subdirectory(libraries)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake_scripts/include_database.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake_scripts/include_compiler_options.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake_scripts/include_output_options.cmake)

#--------------------------------------------------------------------------------
