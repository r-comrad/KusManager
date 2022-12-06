#include "postgresql.hpp"

#ifdef DB_POSTGRESQL

#    include "domain/cyrillic.hpp"

//--------------------------------------------------------------------------------

data::Posdtgres::Posdtgres() noexcept
// -------------------------
// dbname = test_sys       \
// user = sys_user         \
// password = 123321       \
// hostaddr = 127.0.0.1    \
// port = 5432             \
// -------------------------
// dbname = test_sys       \
// user = sys_user         \
// password = 1209root     \
// hostaddr = 127.0.0.1    \
// port = 5432             \
// -------------------------
{
    WRITE_LOG("Creating_postgresql_database_connection");
    try
    {
        mConnexion = new pqxx::connection{" \
            dbname = test_sys       \
            user = sys_user         \
            password = 123321     \
            hostaddr = 127.0.0.1    \
            port = 5432             \
        "};
    }
    catch (const std::exception& e)
    {
        WRITE_ERROR(e.what());
        exit(0);
    }

    WRITE_LOG("Opening_postgresql_database");
    if (mConnexion->is_open())
    {
        WRITE_LOG("Opened_database_successfully");
    }
    else
    {
        WRITE_LOG("Can't_open_database");
        exit(0);
    }
}

//--------------------------------------------------------------------------------

void
data::Posdtgres::select(std::wstring aTableName, std::wstring aColum,
                        std::wstring aConditon, int aStatementID) noexcept
{
    if (aColum == L"") aColum = L"*";
    if (aConditon != L"") aConditon = L" WHERE " + aConditon;
    std::wstring statement =
        L"SELECT " + aColum + L" FROM " + aTableName + aConditon;

    prepare(statement, aStatementID);
}

//--------------------------------------------------------------------------------

void
data::Posdtgres::update(std::wstring aTableName, std::wstring aValue,
                        std::wstring aConditon, int aStatementID) noexcept
{
    std::wstring statement =
        L"UPDATE " + aTableName + L" SET " + aValue + L" WHERE " + aConditon;
    prepare(statement, aStatementID);
}

//--------------------------------------------------------------------------------

int
data::Posdtgres::step(int aStatementID) noexcept
{
    mResultIterator[aStatementID]++;
    return 0;
}

//--------------------------------------------------------------------------------

void
data::Posdtgres::closeStatment(int aStatementID) noexcept
{
    if (mStatement.size() <= aStatementID) return;
    mStatement[aStatementID]->commit();
    mStatement[aStatementID] = nullptr;
    while (mStatement.size() >= 1 && mStatement[aStatementID] == nullptr)
    {
        mStatement.pop_back();
        mResult.pop_back();
        mResultIterator.pop_back();
    }
}

//--------------------------------------------------------------------------------

bool
data::Posdtgres::hasData(int aStatementID) noexcept
{
    // return !mResultIterator[aStatementID].empty();
    return !mResultIterator[aStatementID][0].is_null();
}

//--------------------------------------------------------------------------------

std::optional<dom::CharArray>
data::Posdtgres::getTextFromRow(int aColumNumber, int aStatementID) noexcept
{
    std::optional<dom::CharArray> result = {};
    auto ptr = mResultIterator[aStatementID][aColumNumber].as<const char*>();

    if (ptr != nullptr)
    {
        result = dom::CharArray(ptr);
    }
    return result;
}

//--------------------------------------------------------------------------------

std::optional<std::wstring>
data::Posdtgres::getText16FromRow(int aColumNumber, int aStatementID) noexcept
{
    std::optional<std::wstring> result = {};

    if (!mResultIterator[aStatementID][aColumNumber].is_null())
    {
        auto str =
            mResultIterator[aStatementID][aColumNumber].as<const char*>();

        std::wstring ss = dom::Cyrilic::global.toWString(str);

        result = ss;
    }

    return result;
}

//--------------------------------------------------------------------------------

std::optional<int>
data::Posdtgres::getIntFromRow(int aColumNumber, int aStatementID) noexcept
{
    if (mResultIterator[aStatementID][aColumNumber].is_null()) return -1;
    return mResultIterator[aStatementID][aColumNumber].as<int>();
}

//--------------------------------------------------------------------------------

std::optional<int64_t>
data::Posdtgres::getInt64FromRow(int aColumNumber, int aStatementID) noexcept
{
    // TODO: optional
    if (mResultIterator[aStatementID][aColumNumber].is_null()) return -1;
    return mResultIterator[aStatementID][aColumNumber].as<int64_t>();
}

std::optional<bool>
data::Posdtgres::getBool(int aColumNumber, int aStatementID) noexcept
{
    // TODO: optional

    return mResultIterator[aStatementID][aColumNumber].as<bool>();
}

//--------------------------------------------------------------------------------

// char*
// data::Posdtgres::toAscii(const unsigned char* s) noexcept
// {
//     // TODO: use my defines
//     int cnt = 0;
//     while (s[cnt++])
//         ;
//     char* result    = (char*)malloc(sizeof(char) * (cnt));
//     result[cnt - 1] = 0;
//     for (int i = 0; s[i];)
//         result[i] = s[i++];
//     return result;
//     // TODO end
// }

//--------------------------------------------------------------------------------

void
data::Posdtgres::prepare(std::wstring aStatment, int aStatementID) noexcept
{
    if (mStatement.size() < aStatementID + 1)
    {
        mStatement.resize(aStatementID + 1);
        mResult.resize(aStatementID + 1);
        mResultIterator.resize(aStatementID + 1);
    }

    try
    {
        std::string temp(aStatment.begin(), aStatment.end());
        mStatement[aStatementID] = std::make_unique<pqxx::work>(*mConnexion);
        mResult[aStatementID] =
            pqxx::result(mStatement[aStatementID]->exec(temp.c_str()));
        mResultIterator[aStatementID] = --mResult[aStatementID].begin();
    }
    catch (const std::exception& e)
    {
        WRITE_ERROR(e.what());
    }
}

//--------------------------------------------------------------------------------

#endif // !DB_POSTGRESQL
