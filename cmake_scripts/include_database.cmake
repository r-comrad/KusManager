# Database options
#set(DB DB_SQLITE)
#set(DB DB_POSTGRESQL)

if (${DB} MATCHES DB_POSTGRESQL)
    set(PQXX /usr/include/pqxx)
    find_library(PQXX_LIB pqxx)
    find_library(PQ_LIB pq)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lpqxx -lpq")
endif()

if (${DB} MATCHES DB_SQLITE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE DB_SQLITE)
elseif(${DB} MATCHES DB_POSTGRESQL)
    target_compile_definitions(${PROJECT_NAME} PRIVATE DB_POSTGRESQL)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${PQXX_LIB} ${PQ_LIB})
endif()
