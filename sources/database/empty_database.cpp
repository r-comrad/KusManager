#include "empty_database.hpp"

//--------------------------------------------------------------------------------

data::EmptyDatabase::EmptyDatabase() noexcept
{
}
void data::EmptyDatabase::select(std::string aTableName, std::string aColum,
                                 std::string aConditon,
                                 int aStatementID) noexcept
{
}

void data::EmptyDatabase::update(std::string aTableName, std::string aValue,
                                 std::string aConditon,
                                 int aStatementID) noexcept
{
}

void data::EmptyDatabase::closeStatment(int aStatementID) noexcept
{
}
int data::EmptyDatabase::step(int aStatementID) noexcept
{
    return 0;
}
bool data::EmptyDatabase::hasData(int aStatementID) noexcept
{
    return false;
}
std::optional<dom::CharArray> data::EmptyDatabase::getTextFromRow(
    int aColumNumber, int aStatementID) noexcept
{
    return {};
}
std::optional<std::wstring> data::EmptyDatabase::getText16FromRow(
    int aColumNumber, int aStatementID) noexcept
{
    return {};
}
int data::EmptyDatabase::getIntFromRow(int aColumNumber,
                                       int aStatementID) noexcept
{
    return 0;
}
int64_t data::EmptyDatabase::getInt64FromRow(int aColumNumber,
                                             int aStatementID) noexcept
{
    return 0;
}

char* data::EmptyDatabase::toAscii(const unsigned char* input) noexcept
{
    return nullptr;
}
bool data::EmptyDatabase::getBool(int aColumNumber, int aStatementID) noexcept
{
    return false;
}
//--------------------------------------------------------------------------------
