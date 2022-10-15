#--------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.21.3)
project(ChineseManager VERSION 1.1)

#--------------------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compile options
if (MSVC_VERSION GREATER_EQUAL "1900")
    set(OS BILL_WINDOWS)
    add_compile_options("/std:c++latest")
else()
    set(OS LINUS_LINUX)
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wl,--no-as-needed -ldl")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wl,--no-as-needed -ldl")
    #find_package (Threads)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_DEBUG")
endif()

# Database options
if (0)
    set(DB DB_SQLITE)
else()
    set(DB DB_POSTGRESQL)
endif()

if (${DB} MATCHES DB_POSTGRESQL)
    set(PQXX /usr/include/pqxx)
    find_library(PQXX_LIB pqxx)
    find_library(PQ_LIB pq)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpqxx -lpq")
endif()

# Boost
#ADD_DEFINITIONS("-DBOOST_ALL_NO_LIB")
#find_package(Boost 1.40 COMPONENTS program_options REQUIRED)

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

#target_link_libraries (${PROJECT_NAME} Threads::Threads)

if (${DB} MATCHES DB_SQLITE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE DB_SQLITE)
elseif(${DB} MATCHES DB_POSTGRESQL)
    target_compile_definitions(${PROJECT_NAME} PRIVATE DB_POSTGRESQL)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${PQXX_LIB} ${PQ_LIB})
endif()

#  Compiler defenitions
if (MSVC_VERSION GREATER_EQUAL "1900")
    target_compile_definitions(${PROJECT_NAME} PRIVATE BILL_WINDOWS)
else()
    
    target_compile_definitions(${PROJECT_NAME} PRIVATE LINUS_LINUX)
endif()
target_compile_definitions(${PROJECT_NAME} PRIVATE
                           _DBG_ LOGS_OUTPUT_TYPE=2 ERRORS_OUTPUT_TYPE=4)

# Output options
set(Configuration D)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/$<0:>)
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX _V${CMAKE_PROJECT_VERSION}_${Configuration}.exe)

#--------------------------------------------------------------------------------
