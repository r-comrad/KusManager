#  Compiler defenitions

if (MSVC_VERSION GREATER_EQUAL "1900")
    target_compile_definitions(${PROJECT_NAME} PRIVATE BILL_WINDOWS)
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE LINUS_LINUX)
endif()
target_compile_definitions(${PROJECT_NAME} PRIVATE
                           _DBG_ LOGS_OUTPUT_TYPE=2 ERRORS_OUTPUT_TYPE=4)
