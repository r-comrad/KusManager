#ifndef SQLITE_DATABASE_HPP
#define SQLITE_DATABASE_HPP

#ifdef DB_SQLITE

//--------------------------------------------------------------------------------
#    include <iostream>
#    include <optional>
#    include <vector>

#    include "domain/string.hpp"

#    include "database/string.hpp"

#    include "SQLite/sqlite3.h"

#    include "base_database.hpp"

//--------------------------------------------------------------------------------

namespace data
{
class SQLiteDatabase : public BaseDatabase
{
    using Base      = sqlite3;
    using Statement = sqlite3_stmt;

public:
    SQLiteDatabase(const std::string& aPath) noexcept;
    ~SQLiteDatabase() = default;

    SQLiteDatabase(const SQLiteDatabase& other)            = delete;
    SQLiteDatabase& operator=(const SQLiteDatabase& other) = delete;

    SQLiteDatabase(SQLiteDatabase&& other) noexcept            = default;
    SQLiteDatabase& operator=(SQLiteDatabase&& other) noexcept = default;

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
    void select(std::string&& aTableName, std::string&& aColum = "",
                std::string&& aConditon = "",
                int aStatementID        = 0) noexcept final override;

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
    void update(std::string&& aTableName, std::string&& aValue,
                std::string&& aConditon,
                int aStatementID = 0) noexcept final override;

    /*
    \brief Close the selected statment.
    \param aStatementID The number of the statment array cell
        that should be closed.
        If empty, the first cell (#0) is used.
    */
    void closeStatment(int aStatementID = 0) noexcept final override;

    /*
    \brief Make a step for the selected statment.
    \param aStatementID The number of the statment array cell
        that should make a step.
        If empty, the first cell (#0) is used.
    \return SQLLite step status.
    */
    int step(int aStatementID = 0) noexcept final override;

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
    std::optional<dom::CharArray> getTextFromRow(
        int aColumNumber, int aStatementID = 0) noexcept final override;

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
    dom::CharArray getText16FromRow(
        int aColumNumber, int aStatementID = 0) noexcept final override;

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
    int getIntFromRow(int aColumNumber,
                      int aStatementID = 0) noexcept final override;

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
    int64_t getInt64FromRow(int aColumNumber,
                            int aStatementID = 0) noexcept final override;

    //--------------------------------------------------------------------------------

private:
    Base* mBase;
    std::vector<Statement*> mStatement;

    void prepare(std::string&& aStatment, int aStatementID) noexcept;
};
} // namespace data

//--------------------------------------------------------------------------------

#endif // !DB_SQLITE

#endif // !SQLITE_DATABASE_HPP