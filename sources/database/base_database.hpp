#ifndef BASE_DATABASE_HPP
#define BASE_DATABASE_HPP

//--------------------------------------------------------------------------------

#include <optional>

#include "domain/string.hpp"

//--------------------------------------------------------------------------------

namespace data
{
class BaseDatabase
{
public:
    BaseDatabase() noexcept = default;
    ~BaseDatabase() = default;

    BaseDatabase(const BaseDatabase& other) = delete;
    BaseDatabase& operator=(const BaseDatabase& other) = delete;

    BaseDatabase(BaseDatabase&& other) noexcept = default;
    BaseDatabase& operator=(BaseDatabase&& other) noexcept = default;

    //--------------------------------------------------------------------------------
    //                            DATABASE QUERY FUNCTIONS
    //--------------------------------------------------------------------------------

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
    virtual void select(std::string&& aTableName, std::string&& aColum = "",
                        std::string&& aConditon = "",
                        int aStatementID = 0) noexcept = 0;

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
    virtual void update(std::string&& aTableName, std::string&& aValue,
                        std::string&& aConditon,
                        int aStatementID = 0) noexcept = 0;

    /*
    \brief Close the selected statment.
    \param aStatementID The number of the statment array cell
        that should be closed.
        If empty, the first cell (#0) is used.
    */
    virtual void closeStatment(int aStatementID = 0) noexcept = 0;

    /*
    \brief Make a step for the selected statment.
    \param aStatementID The number of the statment array cell
        that should make a step.
        If empty, the first cell (#0) is used.
    \return SQLLite step status.
    */
    virtual int step(int aStatementID = 0) noexcept = 0;

    //--------------------------------------------------------------------------------
    //                        INFORMATION RECEIVING FUNCTIONS
    //--------------------------------------------------------------------------------

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
        int aColumNumber, int aStatementID = 0) noexcept = 0;

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
    virtual dom::CharArray getText16FromRow(int aColumNumber,
                                            int aStatementID = 0) noexcept = 0;

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
    virtual int getIntFromRow(int aColumNumber,
                              int aStatementID = 0) noexcept = 0;

    /*
    \brief Gets large iteger from colum of current row. The current line
        defined by the statement step.
    \param aColumNumber The number of the column with number to get.
    \param aStatementID Number of statment array cell to use.
        To simultaneously execute multiple queries, you must use
        different cells for each query.
        If empty, the first cell (#0) is used.
    \return Number from specidied cell.
    */
    virtual int64_t getInt64FromRow(int aColumNumber,
                                    int aStatementID = 0) noexcept = 0;
};
} // namespace data

//--------------------------------------------------------------------------------

#endif // !BASE_DATABASE_HPP
