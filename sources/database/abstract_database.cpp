#include "abstract_database.hpp"

//--------------------------------------------------------------------------------

void
data::AbstractDatabase::select(std::wstring aTableName, std::wstring aColum,
                               std::wstring aConditon,
                               int aStatementID) noexcept
{
}

void
data::AbstractDatabase::update(std::wstring aTableName, std::wstring aValue,
                               std::wstring aConditon,
                               int aStatementID) noexcept
{
}

int
data::AbstractDatabase::step(int aStatementID) noexcept
{
    return 0;
}

void
data::AbstractDatabase::closeStatment(int aStatementID) noexcept
{
}

bool
data::AbstractDatabase::hasData(int aStatementID) noexcept
{
    return false;
}

std::optional<dom::CharArray>
data::AbstractDatabase::getTextFromRow(int aColumNumber,
                                       int aStatementID) noexcept
{
    return {};
}

std::optional<std::wstring>
data::AbstractDatabase::getText16FromRow(int aColumNumber,
                                         int aStatementID) noexcept
{
    return {};
}

std::optional<int>
data::AbstractDatabase::getIntFromRow(int aColumNumber,
                                      int aStatementID) noexcept
{
    return 0;
}

std::optional<int64_t>
data::AbstractDatabase::getInt64FromRow(int aColumNumber,
                                        int aStatementID) noexcept
{
    return 0;
}

// std::optional<char*>
// data::EmptyDatabase::toAscii(const unsigned char* input) noexcept
// {
//     return nullptr;
// }

std::optional<bool>
data::AbstractDatabase::getBool(int aColumNumber, int aStatementID) noexcept
{
    return false;
}
//--------------------------------------------------------------------------------
