#--------------------------------------------------------------------------------

macro(get_sources SOURCE_LIST DIR)
    set(FOLDER_NAMES algorithms core database domain main)
    add_sources(${SOURCE_LIST} "${DIR}/sources" "${FOLDER_NAMES}")
endmacro()

#--------------------------------------------------------------------------------
