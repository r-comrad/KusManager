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

    //----------------------------------------------------------------------------
    //							DATABASE QUERY FUNCTIONS
    //----------------------------------------------------------------------------

    /*
    \brief Prepare the sqlite SELECT statement.
    \param aTableName The name of the table for data selection.
    \param aColum The names of the columns to be selected.
        If empty, select all columns.
    \param aConditon Specifies the conditions for the selection request.
        If empty, then no conditions are used.
    \param aStatementID Number of statment array cell to use.
        To simultaneously execute multiple queries, you must use
        different cells for each query.
        If empty, the first cell (#0) is used.
    */
    virtual void select(std::wstring aTableName, std::wstring aColum = L"",
                        std::wstring aConditon = L"",
                        int aStatementID       = 0) noexcept;

    /*
    \brief Prepare the sqlite UPDATE statement.
    \param aTableName The name of the table for data update.
    \param aValue New value for the sqlite table cell.
    \param aConditon Specifies the conditions for data update.
    \param aStatementID Number of statment array cell to use.
        To simultaneously execute multiple queries, you must use
        different cells for each query.
        If empty, the first cell (#0) is used.
    */
    virtual void update(std::wstring aTableName, std::wstring aValue,
                        std::wstring aConditon, int aStatementID = 0) noexcept;

    /*
    \brief Make a step for the selected statment.
    \param aStatementID The number of the statment array cell
        that should make a step.
        If empty, the first cell (#0) is used.
    \return SQLLite step status.
    */
    virtual int step(int aStatementID = 0) noexcept;

    /*
    \brief Close the selected statment.
    \param aStatementID The number of the statment array cell
        that should be closed.
        If empty, the first cell (#0) is used.
    */
    virtual void closeStatment(int aStatementID = 0) noexcept;

    //----------------------------------------------------------------------------
    //						INFORMATION RECEIVING FUNCTIONS
    //----------------------------------------------------------------------------

    virtual bool hasData(int aStatementID = 0) noexcept;

    /*
    \brief Gets UTF-8 string from colum of current row. The current line
        defined by the statement step.
    \param aColumNumber The number of the column with string to get.
    \param aStatementID Number of statment array cell to use.
        To simultaneously execute multiple queries, you must use
        different cells for each query.
        If empty, the first cell (#0) is used.
    \return Pointer to unt-8 string from specidied cell.
    */
    virtual std::optional<dom::CharArray> getTextFromRow(
        int aColumNumber, int aStatementID = 0) noexcept;

    /*
    \brief Gets UTF-16 string from colum of current row. The current line
        defined by the statement step.
    \param aColumNumber The number of the column with string to get.
    \param aStatementID Number of statment array cell to use.
        To simultaneously execute multiple queries, you must use
        different cells for each query.
        If empty, the first cell (#0) is used.
    \return Pointer to unt-8 string from specidied cell.
    */
    virtual std::optional<std::wstring> getText16FromRow(
        int aColumNumber, int aStatementID = 0) noexcept;

    /*
    \brief Gets integer from colum of current row. The current line
        defined by the statement step.
    \param aColumNumber The number of the column with number to get.
    \param aStatementID Number of statment array cell to use.
        To simultaneously execute multiple queries, you must use
        different cells for each query.
        If empty, the first cell (#0) is used.
    \return Number from specidied cell.
    */
    virtual std::optional<int> getIntFromRow(int aColumNumber,
                                             int aStatementID = 0) noexcept;

    /*
    \brief Gets large nteger from colum of current row. The current line
        defined by the statement step.
    \param aColumNumber The number of the column with number to get.
    \param aStatementID Number of statment array cell to use.
        To simultaneously execute multiple queries, you must use
        different cells for each query.
        If empty, the first cell (#0) is used.
    \return Number from specidied cell.
    */
    virtual std::optional<int64_t> getInt64FromRow(
        int aColumNumber, int aStatementID = 0) noexcept;

    // std::optional<char*> toAscii(const unsigned char* input) noexcept;

    virtual std::optional<bool> getBool(int aColumNumber,
                                        int aStatementID = 0) noexcept;
};
} // namespace data

//--------------------------------------------------------------------------------

#endif // !ABSTRACT_DATABASE_HPP
