#--------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.21)

#--------------------------------------------------------------------------------

macro(get_libraries SOURCE_LIST DIR)
    set(FOLDER_NAMES CPUTime)
    add_sources(${SOURCE_LIST} "${DIR}/libraries" "${FOLDER_NAMES}")
endmacro()

#--------------------------------------------------------------------------------
