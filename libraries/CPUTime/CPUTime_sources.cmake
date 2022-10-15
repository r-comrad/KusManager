#--------------------------------------------------------------------------------

set (FILE_NAME ${CMAKE_CURRENT_LIST_DIR}/getCPUTime.cpp)
set (URL https://gist.githubusercontent.com/Randl/45bcca59720f661fa033a67d5f44bff0/raw/ba8764bdc4725f5c411181e65df7330d3abed9af/getCPUTime.c)
set (HASH 91389859f9039f0e6e8bf143d9b3a1e1)

prepare_file(${FILE_NAME} ${URL} ${HASH})

set(
    CPUTIME_SOURCES

    getCPUTime.cpp
    getCPUTime.hpp
)

#--------------------------------------------------------------------------------
