#include "database.hpp"

//--------------------------------------------------------------------------------

data::Database::Database(std::string aDBName) noexcept
    #ifdef DB_SQLITE
    :
    SQLiteDatabase(aDBName)
    #endif
{}

//--------------------------------------------------------------------------------
