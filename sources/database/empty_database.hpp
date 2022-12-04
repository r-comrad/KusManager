#ifndef EMPTY_DATABASE_HPP
#define EMPTY_DATABASE_HPP

//--------------------------------------------------------------------------------

#include <optional>
#include <string>

#include "domain/string.hpp"

//--------------------------------------------------------------------------------

namespace data
{
class EmptyDatabase
{
public:
    EmptyDatabase() noexcept;
    virtual ~EmptyDatabase() = default;

    EmptyDatabase(const EmptyDatabase& other) = delete;
    EmptyDatabase& operator=(const EmptyDatabase& other) = delete;

    EmptyDatabase(EmptyDatabase&& other) noexcept = default;
    EmptyDatabase& operator=(EmptyDatabase&& other) noexcept = default;

    void select(std::string aTableName, std::string aColum = "",
                std::string aConditon = "", int aStatementID = 0) noexcept;

    void update(std::string aTableName, std::string aValue,
                std::string aConditon, int aStatementID = 0) noexcept;

    void closeStatment(int aStatementID = 0) noexcept;
    int step(int aStatementID = 0) noexcept;
    bool hasData(int aStatementID = 0) noexcept;
    std::optional<dom::CharArray> getTextFromRow(int aColumNumber,
                                                 int aStatementID = 0) noexcept;
    std::optional<std::wstring> getText16FromRow(int aColumNumber,
                                                 int aStatementID = 0) noexcept;
    int getIntFromRow(int aColumNumber, int aStatementID = 0) noexcept;
    int64_t getInt64FromRow(int aColumNumber, int aStatementID = 0) noexcept;

    char* toAscii(const unsigned char* input) noexcept;
    bool getBool(int aColumNumber, int aStatementID = 0) noexcept;
};
} // namespace data

//--------------------------------------------------------------------------------

#endif // !EMPTY_DATABASE_HPP
