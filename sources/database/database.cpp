#include "database.hpp"

#include <memory>

//--------------------------------------------------------------------------------

data::Database&
data::Database::getDatabase() noexcept
{
    static std::unique_ptr<Database> database = std::make_unique<Database>();
    return *database.get();
}

//--------------------------------------------------------------------------------
