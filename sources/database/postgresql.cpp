#include "postgresql.hpp"

#ifdef DB_POSTGRESQL

//--------------------------------------------------------------------------------

Posdtgres::Posdtgres() noexcept 
// : 
//     mConnexion
//     {"                          \
//         dbname = test_sys       \
//         user = sys_user         \
//         password = 123321       \
//         hostaddr = 127.0.0.1    \
//         port = 5432             \
//     "}
    // {"                          \
    //     dbname = test_sys       \
    //     user = sys_user         \
    //     password = 1209root     \
    //     hostaddr = 127.0.0.1    \
    //     port = 5432             \
    // "}
{
    WRITE_LOG("Creating_postgresql_database_connection");
    try
    {
        mConnexion = new pqxx::connection
        {"                          \
            dbname = test_sys       \
            user = sys_user         \
            password = 123321     \
            hostaddr = 127.0.0.1    \
            port = 5432             \
        "};
    }
    catch(const std::exception& e)
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
Posdtgres::select
(
    std::string aTableName,
    std::string aColum,
    std::string aConditon,
    int aStatementID
) noexcept
{
    if (aColum == "") aColum = "*";
    if (aConditon != "") aConditon = " WHERE " + aConditon;
    std::string statement = "SELECT " + aColum +
        " FROM " + aTableName + aConditon;
    
    prepare(statement, aStatementID);
}

//--------------------------------------------------------------------------------

void 
Posdtgres::update
(
    std::string aTableName,
    std::string aValue,
    std::string aConditon,
    int aStatementID
) noexcept
{
    std::string statement = "UPDATE " + aTableName + " SET " + aValue +
         " WHERE " + aConditon;
    prepare(statement, aStatementID);
}

//--------------------------------------------------------------------------------

bool 
Posdtgres::hasData(int aStatementID) noexcept
{
    //return !mResultIterator[aStatementID].empty();
    return !mResultIterator[aStatementID][0].is_null();
}

//--------------------------------------------------------------------------------

std::optional<dom::CharArray>
Posdtgres::getTextFromRow(int aColumNumber, int aStatementID) noexcept
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

void destroy(std::wstring& s)
{
    std::vector<std::wstring> data = 
    {
        L"павильон",
        L"царское",
        L"село",
        L"(",
        L")"
    };

    for(auto& ss : data)
    {
        auto it = s.find(ss);
        if (it != -1)
        {
            for(size_t i = 0; i < ss.size(); ++i)
            {
                s[i + it] = '[';
            }
            if (s[it - 1] == ' ')s[it - 1] = '[';
        }
    }
}

#include "domain/cyrillic.hpp"
std::optional<std::wstring>
Posdtgres::getText16FromRow(int aColumNumber, int aStatementID) noexcept
{
    std::optional<std::wstring> result = {};
   
    if (!mResultIterator[aStatementID][aColumNumber].is_null()) 
    {
        auto str = mResultIterator[aStatementID][aColumNumber].as<const char*>();
        // std::cout << mResultIterator[aStatementID][aColumNumber].type();
        // auto ss = mResultIterator[aStatementID][aColumNumber].as_array();
        // while (true)
        // {
        //     auto c =         ss.get_next();
        //     c.second.
        //     int yy;
        //     yy = 0;
        // }

        // exit(0);

        // destroy(ss);

        std::wstring ss = dom::Cyrilic::global.toWString(str);

        result = ss;
    }

    return result;
}

//--------------------------------------------------------------------------------

int 
Posdtgres::getIntFromRow(int aColumNumber, int aStatementID) noexcept
{
    if (mResultIterator[aStatementID][aColumNumber].is_null()) return -1;
    return mResultIterator[aStatementID][aColumNumber].as<int>();
}

//--------------------------------------------------------------------------------

int64_t 
Posdtgres::getInt64FromRow(int aColumNumber, int aStatementID) noexcept
{
    //TODO: optional
        if (mResultIterator[aStatementID][aColumNumber].is_null()) return -1;
        return mResultIterator[aStatementID][aColumNumber].as<int64_t>();
}

bool 
Posdtgres::getBool(int aColumNumber, int aStatementID) noexcept
{
    //TODO: optional

        return mResultIterator[aStatementID][aColumNumber].as<bool>();
}


//--------------------------------------------------------------------------------

void 
Posdtgres::closeStatment(int aStatementID) noexcept
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

int 
Posdtgres::step(int aStatementID) noexcept
{
    mResultIterator[aStatementID]++;
    return 0;
}

//--------------------------------------------------------------------------------

char* 
Posdtgres::toAscii(const unsigned char* s) noexcept
{
    //TODO: use my defines
    int cnt = 0;
    while (s[cnt++]);
    char* result = (char*) malloc(sizeof(char) * (cnt));
    result[cnt - 1] = 0;
    for (int i = 0; s[i];) result[i] = s[i++];
    return result;
    //TODO end
}

//--------------------------------------------------------------------------------

void 
Posdtgres::prepare(std::string aStatment, int aStatementID) noexcept
{
    if (mStatement.size() < aStatementID + 1)
    {
        mStatement.resize(aStatementID + 1);
        mResult.resize(aStatementID + 1);
        mResultIterator.resize(aStatementID + 1);
    }

    try
    {
        mStatement[aStatementID] = 
            std::make_unique<pqxx::work>(*mConnexion);
        mResult[aStatementID] = pqxx::result( mStatement[aStatementID]->
            exec(aStatment.c_str()));
        mResultIterator[aStatementID] = --mResult[aStatementID].begin();
    }
    catch(const std::exception& e)
    {
        WRITE_ERROR(e.what());
    }
}

//--------------------------------------------------------------------------------

#endif // !DB_POSTGRESQL
