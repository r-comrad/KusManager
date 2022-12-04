#ifndef DATABASE_QUERY_HPP
#define DATABASE_QUERY_HPP

//--------------------------------------------------------------------------------

#include <map>
#include <mutex>
#include <optional>
#include <vector>

#include "domain/string.hpp"

#include "database.hpp"

#include "submission_info.hpp"

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
    struct CompetitionData
    {
        uint32_t id;
        // startTime
        // endTime
    };
    struct QuestionData
    {
        uint32_t id;
        // startTime
        // endTime
    };
    struct UserAnswers
    {
        uint32_t id;
        std::vector<std::string> ans;
    };

    // struct User
    // {
    //     int id;
    //     std::string name;
    // };
    using UserNames = std::map<int, std::string>;

public:
    /*
    \brief Construct querys handler for base with specifien name
    \param aDatabasePath Path to database.
    */
    DatabaseQuery(const std::string& aDatabasePath) noexcept;
    ~DatabaseQuery() = default;

    DatabaseQuery(const DatabaseQuery& other) = delete;
    DatabaseQuery& operator=(const DatabaseQuery& other) = delete;

    DatabaseQuery(DatabaseQuery&& other) noexcept = default;
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
    void writeResult(int aID, const std::string& aResult, int aTime,
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

    CompetitionData getCompetitionInfo(
        const std::string& aCompetitionName) noexcept;
    // void getQuestionInfo(int aCompetitionID) noexcept;
    std::vector<int> getQuestionNumbers(int aCompetitionID) noexcept;
    std::map<std::wstring, int> getQuestionNames(
        const std::vector<int>& aIDs) noexcept;
    std::map<int, std::wstring> getQuestions(
        const std::vector<int>& aQuestionNumbers) noexcept;
    std::vector<int> getGroupIDs(int aCompetitionID) noexcept;
    std::vector<int> getUserIDs(const std::vector<int>& aGroups) noexcept;
    std::map<int, std::wstring> getUserNames(
        const std::vector<int>& aUserIDs) noexcept;
    std::map<int, std::map<int, std::wstring>> getUserAnswers(
        const std::vector<int>& aUserIDs,
        const std::vector<int>& aQuestionNumbers) noexcept;

    std::map<int, std::wstring> getUserAnswers(
        const std::vector<int>& aQuaestionIDs, int aUserId) noexcept;

    void rename(const std::map<int, std::string>& aOldNames,
                std::vector<std::string>&& aNewNames,
                bool aOnlyTest = false) noexcept;
    void turnOff(std::vector<std::string> nameTemplate) noexcept;
    void turnOn() noexcept;

    //------------------

    bool isUserHasBron(const std::string& aName);

    UserNames getUsers(const std::vector<std::string>& aMask = {},
                       bool aSwitchMask = false);
    UserNames getActiveUsers(const std::vector<std::string>& aMask = {},
                             bool aSwitchMask = false);
    UserNames getDeletedUsers(const std::vector<std::string>& aMask = {},
                              bool aSwitchMask = false);

    std::map<std::string, std::string> getPasswords(UserNames aUsers);

private:
    Database mDatabase;
    int mReservedStatementNumber;

    std::mutex mTestMutex;
    uint32_t mTestNum;
    bool mTestAreOver;

    void getParticipantInfo(SubmissionInfo& aSubmissionInfo) noexcept;
    void getCheckerInfo(SubmissionInfo& aSubmissionInfo) noexcept;
    UserNames getUsersByDeleteFlag(bool aIsDeleted,
                                   const std::vector<std::string>& aMask = {},
                                   bool aSwitchMask = false);
};
} // namespace data

//--------------------------------------------------------------------------------

#endif // !DATABASE_QUERY_HPP
