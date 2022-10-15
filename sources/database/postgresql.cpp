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
            password = 1209root     \
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

void to_lower(std::wstring& s)
{
    for(auto& i : s)
    {
        if (i == L'А') i = L'а';
        if (i == L'Б') i = L'б';
        if (i == L'В') i = L'в';
        if (i == L'Г') i = L'г';
        if (i == L'Д') i = L'д';
        if (i == L'Е') i = L'е';
        if (i == L'Ё') i = L'е';
        if (i == L'Ж') i = L'ж';
        if (i == L'З') i = L'з';
        if (i == L'И') i = L'и';
        if (i == L'Й') i = L'й';
        if (i == L'К') i = L'к';
        if (i == L'Л') i = L'л';
        if (i == L'М') i = L'м';
        if (i == L'Н') i = L'н';
        if (i == L'О') i = L'о';
        if (i == L'П') i = L'п';
        if (i == L'Р') i = L'р';
        if (i == L'С') i = L'с';
        if (i == L'Т') i = L'т';
        if (i == L'У') i = L'у';
        if (i == L'Ф') i = L'ф';
        if (i == L'Х') i = L'х';
        if (i == L'Ц') i = L'ц';
        if (i == L'Ч') i = L'ч';
        if (i == L'Ш') i = L'ш';
        if (i == L'Щ') i = L'щ';
        if (i == L'Ъ') i = L'ъ';
        if (i == L'Ы') i = L'ы';
        if (i == L'Ь') i = L'ь';
        if (i == L'Э') i = L'э';
        if (i == L'Ю') i = L'ю';
        if (i == L'Я') i = L'я';
        if (i == L'ё') i = L'е';
        if (i == L'й') i = L'и';
    }
}
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
        auto str2 = const_cast<char*>(str);
        u_char* s = (u_char*)str2;
        std::wstring ss;
        for(u_char* c(s); *c !='\000'; ++c)
        {
            if (*c == 208 || *c == 209)
            {
                ss.push_back((*c==208 ? 896 : 960) + *++c);
            }
            else
            {
                ss.push_back(static_cast<wchar_t>(*c));
            }
        }
        to_lower(ss);
        destroy(ss);
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
