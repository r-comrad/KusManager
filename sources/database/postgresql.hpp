#ifndef POSTGRESQL_HPP
#define POSTGRESQL_HPP

#ifdef DB_POSTGRESQL

//--------------------------------------------------------------------------------

#    include <pqxx/pqxx>

#    include <memory>
#    include <optional>
#    include <vector>

#    include "domain/error_message.hpp"
#    include "domain/string.hpp"

#    include "abstract_database.hpp"
//--------------------------------------------------------------------------------
namespace data
{
class Posdtgres : public AbstractDatabase
{
public:
    Posdtgres() noexcept;
    virtual ~Posdtgres() = default;

    Posdtgres(const Posdtgres& other)            = delete;
    Posdtgres& operator=(const Posdtgres& other) = delete;

    Posdtgres(Posdtgres&& other) noexcept            = default;
    Posdtgres& operator=(Posdtgres&& other) noexcept = default;

    //----------------------------------------------------------------------------
    //							DATABASE QUERY FUNCTIONS
    //----------------------------------------------------------------------------

    void select(std::wstring aTableName, std::wstring aColum = L"",
                std::wstring aConditon = L"",
                int aStatementID       = 0) noexcept final;
    void update(std::wstring aTableName, std::wstring aValue,
                std::wstring aConditon, int aStatementID = 0) noexcept final;

    int step(int aStatementID = 0) noexcept final;
    void closeStatment(int aStatementID = 0) noexcept final;
    bool hasData(int aStatementID = 0) noexcept final;

    std::optional<dom::CharArray> getTextFromRow(
        int aColumNumber, int aStatementID = 0) noexcept final;

    std::optional<std::wstring> getText16FromRow(
        int aColumNumber, int aStatementID = 0) noexcept final;

    std::optional<int> getIntFromRow(int aColumNumber,
                                     int aStatementID = 0) noexcept final;
    std::optional<int64_t> getInt64FromRow(int aColumNumber,
                                           int aStatementID = 0) noexcept final;

    std::optional<bool> getBool(int aColumNumber,
                                int aStatementID = 0) noexcept final;

private:
    pqxx::connection* mConnexion;
    std::vector<std::unique_ptr<pqxx::work>> mStatement;
    std::vector<pqxx::result> mResult;
    std::vector<pqxx::result::const_iterator> mResultIterator;

    void prepare(std::wstring aStatment, int aStatementID) noexcept;
};
} // namespace data

//--------------------------------------------------------------------------------

#endif // !DB_POSTGRESQL

#endif // !POSTGRESQL_HPP
