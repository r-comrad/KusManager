#--------------------------------------------------------------------------------

add_library(
    SQLite

    shell.c
    sqlite3.c

    sqlite3.h
    sqlite3ext.h
)

set(
    SQLITE_SOURCES

    shell.c
    sqlite3.c

    sqlite3.h
    sqlite3ext.h
)

set(
    FILE_HASHS

    "c39bced5df38bbc66ff841d2c21856cc"
    "86ed5da8b9d586252181f07c3619d377"

    "d4c76f83bc0efb24c0732daab86c913b"
    "f24ca63ce331bcd2fde20e5f40cfc7ae"
)

set (URL https://www.sqlite.org/2022/sqlite-amalgamation-3390200.zip)

prepare_lib(${URL} ${CMAKE_CURRENT_LIST_DIR} "${SQLITE_SOURCES}" "${FILE_HASHS}" )

target_compile_definitions(${PROJECT_NAME} PRIVATE SQLITE)

#--------------------------------------------------------------------------------
