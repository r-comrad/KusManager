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

#    include "base_database.hpp"
//--------------------------------------------------------------------------------

class Posdtgres
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
    void select(std::string aTableName, std::string aColum = "",
                std::string aConditon = "", int aStatementID = 0) noexcept;

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
    void update(std::string aTableName, std::string aValue,
                std::string aConditon, int aStatementID = 0) noexcept;

    /*
    \brief Close the selected statment.
    \param aStatementID The number of the statment array cell
        that should be closed.
        If empty, the first cell (#0) is used.
    */
    void closeStatment(int aStatementID = 0) noexcept;

    /*
    \brief Make a step for the selected statment.
    \param aStatementID The number of the statment array cell
        that should make a step.
        If empty, the first cell (#0) is used.
    \return SQLLite step status.
    */
    int step(int aStatementID = 0) noexcept;

    //----------------------------------------------------------------------------
    //						INFORMATION RECEIVING FUNCTIONS
    //----------------------------------------------------------------------------

    bool hasData(int aStatementID = 0) noexcept;

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
    std::optional<dom::CharArray> getTextFromRow(int aColumNumber,
                                                 int aStatementID = 0) noexcept;

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
    std::optional<std::wstring> getText16FromRow(int aColumNumber,
                                                 int aStatementID = 0) noexcept;

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
    int getIntFromRow(int aColumNumber, int aStatementID = 0) noexcept;

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
    int64_t getInt64FromRow(int aColumNumber, int aStatementID = 0) noexcept;

    //----------------------------------------------------------------------------

    /*
    \brief Gets Integer from colum of current row. The current line
        defined by the statement step.
    \param aColumNumber The number of the column with number to get.
    \param aStatementID Number of statment array cell to use.
        To simultaneously execute multiple queries, you must use
        different cells for each query.
        If empty, the first cell (#0) is used.
    \return Number from specidied cell.
    */
    char* toAscii(const unsigned char* input) noexcept;
    bool getBool(int aColumNumber, int aStatementID = 0) noexcept;

private:
    pqxx::connection* mConnexion;
    std::vector<std::unique_ptr<pqxx::work>> mStatement;
    std::vector<pqxx::result> mResult;
    std::vector<pqxx::result::const_iterator> mResultIterator;

    void prepare(std::string aStatment, int aStatementID) noexcept;
};

//--------------------------------------------------------------------------------

#endif // !DB_POSTGRESQL

#endif // !POSTGRESQL_HPP
