#ifndef DATABASE_HPP
#define DATABASE_HPP

//--------------------------------------------------------------------------------

#include <string>

#include "abstract_database.hpp"
#include "postgresql.hpp"
#include "sqlite_database.hpp"

//--------------------------------------------------------------------------------

namespace data
{
#if defined(DB_SQLITE)
class Database : public SQLiteDatabase
#elif defined(DB_POSTGRESQL)
class Database : public Posdtgres
#else
class Database : public AbstractDatabase
#endif
{
public:
    Database() noexcept = default;
    virtual ~Database() = default;

    Database(const Database& other)            = delete;
    Database& operator=(const Database& other) = delete;

    Database(Database&& other) noexcept            = default;
    Database& operator=(Database&& other) noexcept = default;
};
} // namespace data

//--------------------------------------------------------------------------------

#endif // !DATABASE_HPP
