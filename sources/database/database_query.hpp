#ifndef DATABASE_QUERY_HPP
#define DATABASE_QUERY_HPP

//--------------------------------------------------------------------------------

#include <map>
#include <mutex>
#include <optional>
#include <vector>

#include "domain/string.hpp"

#include "database.hpp"

#include "contest.hpp"
#include "submission_info.hpp"
#include "user.hpp"

//--------------------------------------------------------------------------------

namespace data
{
class DatabaseQuery
{
private:
    struct TestData
    {
        dom::CharArray input;
        dom::CharArray output;
        uint32_t testNum;
    };

public:
    /*
    \brief Construct querys handler for base with specifien name
    \param aDatabasePath Path to database.
    */
    DatabaseQuery() noexcept;
    ~DatabaseQuery() = default;

    DatabaseQuery(const DatabaseQuery& other)            = delete;
    DatabaseQuery& operator=(const DatabaseQuery& other) = delete;

    DatabaseQuery(DatabaseQuery&& other) noexcept            = default;
    DatabaseQuery& operator=(DatabaseQuery&& other) noexcept = default;

    /*
    \brief Retrieves the problem information (ID, file names, restrictions)
    from the database and puts it in the SubmissionInfo structure.
    */
    SubmissionInfo getSubmissionInfo(int ID) noexcept;

    /*
    \brief Writing result of submission check in database.
    \param aID Submission ID.
    \param aResult Result of submission check.
    \param aTime Submission time usage.
    \param aMemory Submission memory usage.
    */
    void writeResult(int aID, const std::wstring& aResult, int aTime,
                     int aMemory) noexcept;

    /*
    \brief Retrieves the next test from the database and puts it in
        TestLibMessage struct.
        (according to current statment step and
        numbering of tests in the database)
    \param aSudmissionInformation Problem info (ID) for test search.
    \param TestLibMessage  TestLibMessage structure for obtaining test.
    */
    std::optional<TestData> getNextTest() noexcept;

    void prepareTestsStatement(uint64_t aProblemID) noexcept;

    //--------------------------------------------------------------------------------

    Contest getCompetition(const std::wstring& aCompetitionName,
                           const std::wstring& aGroupName = L"") noexcept;

    std::vector<int> getUserIDs(const std::vector<int>& aGroups) noexcept;
    std::map<int, std::map<int, std::wstring>> getUserAnswers(
        const std::vector<int>& aUserIDs,
        const std::vector<int>& aQuestionNumbers) noexcept;

private:
    Database& mDatabase;
    int mReservedStatementNumber;

    std::mutex mTestMutex;
    uint32_t mTestNum;
    bool mTestAreOver;

    std::vector<int> getQuestionNumbers(int aCompetitionID) noexcept;

    void getParticipantInfo(SubmissionInfo& aSubmissionInfo) noexcept;
    void getCheckerInfo(SubmissionInfo& aSubmissionInfo) noexcept;

};
} // namespace data

//--------------------------------------------------------------------------------

#endif // !DATABASE_QUERY_HPP
