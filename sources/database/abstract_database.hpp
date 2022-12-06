#ifndef ABSTRACT_DATABASE_HPP
#define ABSTRACT_DATABASE_HPP

//--------------------------------------------------------------------------------

#include <optional>
#include <string>

#include "domain/string.hpp"

//--------------------------------------------------------------------------------

namespace data
{
class AbstractDatabase
{
public:
    AbstractDatabase() noexcept = default;
    virtual ~AbstractDatabase() = default;

    AbstractDatabase(const AbstractDatabase& other)            = delete;
    AbstractDatabase& operator=(const AbstractDatabase& other) = delete;

    AbstractDatabase(AbstractDatabase&& other) noexcept            = default;
    AbstractDatabase& operator=(AbstractDatabase&& other) noexcept = default;

    void select(std::wstring aTableName, std::wstring aColum = L"",
                std::wstring aConditon = L"", int aStatementID = 0) noexcept;

    void update(std::wstring aTableName, std::wstring aValue,
                std::wstring aConditon, int aStatementID = 0) noexcept;

    void closeStatment(int aStatementID = 0) noexcept;
    int step(int aStatementID = 0) noexcept;
    bool hasData(int aStatementID = 0) noexcept;
    std::optional<dom::CharArray> getTextFromRow(int aColumNumber,
                                                 int aStatementID = 0) noexcept;
    std::optional<std::wstring> getText16FromRow(int aColumNumber,
                                                 int aStatementID = 0) noexcept;
    std::optional<int> getIntFromRow(int aColumNumber,
                                     int aStatementID = 0) noexcept;
    std::optional<int64_t> getInt64FromRow(int aColumNumber,
                                           int aStatementID = 0) noexcept;

    // std::optional<char*> toAscii(const unsigned char* input) noexcept;
    std::optional<bool> getBool(int aColumNumber,
                                int aStatementID = 0) noexcept;
};
} // namespace data

//--------------------------------------------------------------------------------

#endif // !ABSTRACT_DATABASE_HPP
