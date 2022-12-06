#include "database/database_query.hpp"

#include <vector>

//--------------------------------------------------------------------------------

#include "domain/error_message.hpp"

//--------------------------------------------------------------------------------

data::DatabaseQuery::DatabaseQuery() noexcept
    : mReservedStatementNumber(0), mTestNum(1), mTestAreOver(false)
{
    WRITE_LOG("Creating_database_quare");
}

//--------------------------------------------------------------------------------

data::SubmissionInfo
data::DatabaseQuery::getSubmissionInfo(int ID) noexcept
{
    SubmissionInfo result;
    result.ID = ID;

    getParticipantInfo(result);
    getCheckerInfo(result);

#ifdef LINUS_LINUX
    result.timeMemLim.timeLimit /= 1000;
#endif

    return result;
}

//--------------------------------------------------------------------------------

void
data::DatabaseQuery::writeResult(int aID, const std::wstring& aResult,
                                 int aTime, int aMemory) noexcept
{
    mDatabase.closeStatment(0);

    START_LOG_BLOCK("Updating_database");
    WRITE_LOG("Updating...");

    mDatabase.update(L"core_solutions",
                     L"result = '" + aResult + L"'" + L", time = " +
                         std::to_wstring(aTime) + L", memory = " +
                         std::to_wstring(aMemory),
                     L"id = " + std::to_wstring(aID));
    mDatabase.step();
    mDatabase.closeStatment();

    END_LOG_BLOCK("Database_updated?");
}

//--------------------------------------------------------------------------------

std::optional<data::DatabaseQuery::TestData>
data::DatabaseQuery::getNextTest() noexcept
{
    std::optional<TestData> result;

    TestData data;
    uint32_t tempNum;
    std::optional<dom::CharArray> input;
    std::optional<dom::CharArray> output;

    mTestMutex.lock();
    if (!mTestAreOver)
    {
        WRITE_LOG("Taking_next_test");
        // if (mDatabase.step() != SQLITE_OK) break; TODO: fixing that
        mDatabase.step(mReservedStatementNumber);
        input   = mDatabase.getTextFromRow(0, mReservedStatementNumber);
        output  = mDatabase.getTextFromRow(1, mReservedStatementNumber);
        tempNum = mTestNum++;

        if (!input.has_value())
        {
            mTestAreOver = true;
        }
    }
    mTestMutex.unlock();

    if (input.has_value())
    {
        data.input   = std::move(input.value());
        data.output  = std::move(output.value());
        data.testNum = tempNum;
        result       = std::make_optional<TestData>(std::move(data));
    }

    return result;
}

//--------------------------------------------------------------------------------

void
data::DatabaseQuery::prepareTestsStatement(uint64_t aProblemID) noexcept
{
    START_LOG_BLOCK("Prepare_geting_test_from_database");
    mDatabase.select(L"core_test", L"input, output",
                     L"contest_id = " + std::to_wstring(aProblemID),
                     mReservedStatementNumber);
    END_LOG_BLOCK("I'm_ready");
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

void
data::DatabaseQuery::rename(const UserArray& aUsers,
                            const std::vector<std::wstring>& aNewNames,
                            bool aOnlyTest) noexcept
{
    auto itUsers    = aUsers.begin();
    auto itNewNames = aNewNames.begin();
    while (itUsers != aUsers.end() && itNewNames != aNewNames.end())
    {
        if (aNewNames.size() == 0) break;
        wprintf_s(L"%s   ----->   %s\n", itUsers->second.name.c_str(),
                  itNewNames->c_str());

        if (!aOnlyTest)
        {
            mDatabase.update(L"core_users",
                             L"username = \'" + *itNewNames + L"\'",
                             L"id = " + std::to_wstring(itUsers->first));
            mDatabase.step();
            mDatabase.closeStatment();
        }

        itUsers++;
        itNewNames++;
    }
}

data::UserArray
data::DatabaseQuery::getUsers(const std::vector<Mask>& aMask,
                              bool aSwitchMask) noexcept
{
    data::UserArray result;

    mDatabase.select(L"core_users", L"id, username, is_superuser, "
                                    L"is_teacher, is_staff");
    while (true)
    {
        mDatabase.step();
        if (!mDatabase.hasData()) break;

        if (mDatabase.getBool(2)) continue;
        if (mDatabase.getBool(3)) continue;
        if (mDatabase.getBool(4)) continue;

        auto id   = mDatabase.getIntFromRow(0).value();
        auto name = mDatabase.getText16FromRow(1).value();

        if (isUserHasBron(name)) continue;

        bool flagOr  = false;
        bool flagAnd = true;
        for (auto& i : aMask)
        {
            bool temp = name.find(i.value);
            if (i.funk & int(Mask::Function::NOT)) temp ^= true;
            if (i.funk & int(Mask::Function::AND)) flagAnd &= temp;
            else flagOr |= temp;
        }

        if ((flagOr && flagAnd) ^ (!aSwitchMask)) continue;

        result.setName(id, std::move(name));
    }
    mDatabase.closeStatment();

    return result;
}

data::UserArray
data::DatabaseQuery::getUsers(const std::vector<int>& aUserIDs) noexcept
{
    data::UserArray result;

    for (auto user : aUserIDs)
    {
        mDatabase.select(L"core_users",
                         L"username, is_superuser, is_teacher, is_staff",
                         L"id = " + std::to_wstring(user));

        if (mDatabase.getBool(2)) continue;
        if (mDatabase.getBool(3)) continue;
        if (mDatabase.getBool(4)) continue;

        mDatabase.step();
        auto name = mDatabase.getText16FromRow(0);
        mDatabase.closeStatment();
        if (!name.has_value()) break;

        if (isUserHasBron(name.value())) continue;
        result.setName(user, std::move(name.value()));
    }

    return result;
}

//--------------------------------------------------------------------------------

Contest
data::DatabaseQuery::getCompetition(const std::wstring& aCompetitionName,
                                    const std::wstring& aGroupName) noexcept
{
    Contest result;
    // START_LOG_BLOCK("Prepare_geting_test_from_database");

    mDatabase.select(L"core_competitions", L"id, start_time, end_time",
                     L"name = \'" + aCompetitionName + L"\'");
    mDatabase.step();
    auto id = mDatabase.getInt64FromRow(0);
    mDatabase.closeStatment();

    if (!id.has_value())
    {
        result.mCompetitionId = 1;
    }
    else
    {
        result.mCompetitionId   = id.value();
        result.mCompetitionName = aCompetitionName;

        if (aGroupName == L"")
        {
            mDatabase.select(
                L"core_studentgroup_competitions", L"studentgroup_id",
                L"competitions_id = " + L"\'"s +
                    std::to_wstring(result.mCompetitionId) + L"\'");

            while (true)
            {
                mDatabase.step();
                auto num       = mDatabase.getInt64FromRow(0);
                auto groupName = ;

                if (!num.has_value()) break;
                result.mGroups[num.value()] = groupName;
            }
            mDatabase.closeStatment();
        }
    }
    //--------------------------------------------------------------------------------
    mDatabase.select(L"core_competitions_questions", L"question_id",
                     L"competitions_id = " + L"\'"s +
                         std::to_wstring(result.mCompetitionId) + L"\'");

    while (true)
    {

        mDatabase.step();
        auto num = mDatabase.getInt64FromRow(0);
        if (!num.has_value()) break;
        result.mQuaestions[num.value()];
    }
    mDatabase.closeStatment();
    //--------------------------------------------------------------------------------

    for (auto question : result.mQuaestions)
    {
        mDatabase.select(L"core_question", L"question",
                         L"id = \'" + std::to_wstring(question.first) + L"\'");

        while (true)
        {
            mDatabase.step();
            auto ans = mDatabase.getText16FromRow(0);

            if (!ans.has_value())
            {
                mDatabase.closeStatment();
                break;
            }
            std::wstring s = ans.value();

            s.resize(s.size() - 4);
            std::wstring ss;
            std::reverse(s.begin(), s.end());
            s.resize(s.size() - 23);
            if (s.back() == L'\"')
            {
                for (auto& i : s)
                {
                    if (i == L' ') i = L'_';
                }
                for (auto& i : s)
                {
                    if (i == L'\"' || i == L'\\' || i == L'[' || i == L']' ||
                        i == L':' || i == L',')
                        i = L' ';
                }
                std::reverse(s.begin(), s.end());

                std::wstringstream sss;
                sss << s;
                for (int i = 0; sss >> s; ++i)
                {
                    sss >> s;
                    sss >> s;
                    if (s == L"true") ss += std::to_wstring(i) + L" ";
                }

                // ss = s;
            }
            else
            {
                std::reverse(s.begin(), s.end());
                ss = s;
            }
            result[questionNum] = ss;
            dom::Cyrilic::global.standardProcedure(result[questionNum]);
        }
        mDatabase.closeStatment();
    }

    for (auto& id : aIDs)
    {
        mDatabase.select(L"core_question", L"name",
                         L"id = " + std::to_string(id));
        mDatabase.step();
        result[mDatabase.getText16FromRow(0).value()] = id;
        mDatabase.closeStatment();
    }

    return result;
}

//--------------------------------------------------------------------------------

std::map<int, std::map<int, std::wstring>>
data::DatabaseQuery::getUserAnswers(
    const std::vector<int>& aUserIDs,
    const std::vector<int>& aQuestionNumbers) noexcept
{
    // CompetitionData result;
    // START_LOG_BLOCK("Prepare_geting_test_from_database");

    std::map<int, std::map<int, std::wstring>> result;

    for (auto question : aQuestionNumbers)
    {
        for (auto user : aUserIDs)
        {
            mDatabase.select("core_questionans", "ans",
                             "question_id = " + std::to_string(question) +
                                 " AND user_id = " + std::to_string(user));

            while (true)
            {

                mDatabase.step();
                auto ans = mDatabase.getText16FromRow(0);

                if (!ans.has_value())
                {
                    mDatabase.closeStatment();
                    break;
                }
                result[user][question] = ans.value();
                dom::Cyrilic::global.standardProcedure(result[user][question]);
            }
            mDatabase.closeStatment();
        }
    }
    // std::cout << "core_questionans:" << "\n";
    // for(auto i : result)
    // {
    //     std::cout << i.first << ":" << "\n";
    //     for(auto j : i.second)
    //     {
    //         std::cout << "\t" << j.first  << " - " << j.second << "\n";
    //     }
    // }
    // std::cout << "\n\n";
    // std::cout << "size: " << result.size() << "\n\n";

    return result;

    // core_questionans

    //   END_LOG_BLOCK("I'm_ready");
    //  return result;
}

// std::map<int, std::wstring>
// data::DatabaseQuery::getUserAnswers(const std::vector<int>& aQuaestionIDs,
//                                     int aUserId) noexcept
// {
//     std::map<int, std::wstring> result;
//     for (auto question : aQuaestionIDs)
//     {

//         mDatabase.select("core_questionans", "ans",
//                          "question_id = " + std::to_string(question) +
//                              " AND user_id = " + std::to_string(aUserId));

//         while (true)
//         {

//             mDatabase.step();
//             auto ans = mDatabase.getText16FromRow(0);

//             if (!ans.has_value())
//             {
//                 mDatabase.closeStatment();
//                 break;
//             }
//             result[question] = ans.value();
//             dom::Cyrilic::global.standardProcedure(result[question]);
//         }
//         mDatabase.closeStatment();
//     }
//     return result;
// }

//--------------------------------------------------------------------------------

bool
data::DatabaseQuery::isUserHasBron(const std::string& aName)
{
    return aName == "pashs" || aName == "ITMO-Student" ||
           aName == "AlphaPrimus" || aName == "Ivan" ||
           aName == "Ivanus-Primus";
}

//--------------------------------------------------------------------------------

// data::UserArray
// data::DatabaseQuery::getUsers(const std::vector<int>& aUserIDs) noexcept
// {
//     data::UserArray result;

//     for (auto user : aUserIDs)
//     {
//         mDatabase.select("core_users", "username",
//                          "id = " + std::to_string(user));

//         mDatabase.step();
//         auto name = mDatabase.getText16FromRow(0);

//         if (!name.has_value())
//         {
//             mDatabase.closeStatment();
//             break;
//         }
//         result.setName(user, name.value());
//         mDatabase.closeStatment();
//     }

//     return result;
// }

void
data::DatabaseQuery::getPasswords(data::UserArray& aUsers) noexcept
{
    for (auto& i : aUsers)
    {
        mDatabase.select("core_passwords", "password",
                         "user_id = " + std::to_string(i.first));

        mDatabase.step();
        i.second.password = mDatabase.getTextFromRow(0).value().getString();
        mDatabase.closeStatment();
    }
}

void
data::DatabaseQuery::getUserAnswers(const std::vector<int>& aQuaestionIDs,
                                    data::UserArray& aUsers) noexcept
{
    for (auto& user : aUsers)
    {
        for (auto question : aQuaestionIDs)
        {

            mDatabase.select("core_questionans", "ans",
                             "question_id = " + std::to_string(question) +
                                 " AND user_id = " + std::to_string(aUserId));

            while (true)
            {

                mDatabase.step();
                auto ans = mDatabase.getText16FromRow(0);

                if (!ans.has_value())
                {
                    mDatabase.closeStatment();
                    break;
                }
                result[question] = ans.value();
                dom::Cyrilic::global.standardProcedure(result[question]);
            }
            mDatabase.closeStatment();
        }
    }
}

//--------------------------------------------------------------------------------

data::DatabaseQuery::UserNames
data::DatabaseQuery::getActiveUsers(const std::vector<std::string>& aMask,
                                    bool aSwitchMask)
{
    return getUsersByDeleteFlag(false, aMask, aSwitchMask);
}

//--------------------------------------------------------------------------------

data::DatabaseQuery::UserNames
data::DatabaseQuery::getDeletedUsers(const std::vector<std::string>& aMask,
                                     bool aSwitchMask)
{
    return getUsersByDeleteFlag(true, aMask, aSwitchMask);
}

//--------------------------------------------------------------------------------

void
data::DatabaseQuery::getParticipantInfo(
    SubmissionInfo& aSubmissionInfo) noexcept
{
    START_LOG_BLOCK("Geting_ID_and_name_from_database");

    mDatabase.select("core_solutions", "file_name, contest_id",
                     "id = " + std::to_string(aSubmissionInfo.ID));
    mDatabase.step();

    aSubmissionInfo.problemID = mDatabase.getInt64FromRow(1);

    auto fileName = mDatabase.getTextFromRow(0);
    if (fileName.has_value())
    {
        aSubmissionInfo.solutionFileName = std::move(fileName.value());
    }
    else
    {
        WRITE_ERROR("DatabaseQuery", "getParticipantInfo", 10,
                    "Can't_take_solution_filename_from_database");
    }
    mDatabase.closeStatment();

    WRITE_LOG("Problem_ID:", aSubmissionInfo.problemID);
    END_LOG_BLOCK("File_name:", aSubmissionInfo.solutionFileName);
}

//--------------------------------------------------------------------------------

void
data::DatabaseQuery::getCheckerInfo(SubmissionInfo& aSubmissionInfo) noexcept
{
    START_LOG_BLOCK("Geting_limits_from_database");

    mDatabase.select("core_contests", "time_limit, memory_limit, checker",
                     "id = " + std::to_string(aSubmissionInfo.problemID));
    mDatabase.step();
    aSubmissionInfo.timeMemLim = {uint64_t(mDatabase.getInt64FromRow(0)),
                                  uint64_t(mDatabase.getInt64FromRow(1))};

    auto fileName = mDatabase.getTextFromRow(2);
    if (fileName.has_value())
    {
        aSubmissionInfo.checkerFileName = std::move(fileName.value());
    }
    else
    {
        WRITE_ERROR("DatabaseQuery", "getParticipantInfo", 10,
                    "Can't_take_checker_filename_from_database");
    }

    mDatabase.closeStatment();

    WRITE_LOG("Time_limit:", aSubmissionInfo.timeMemLim.timeLimit);
    END_LOG_BLOCK("Memory_limit:", aSubmissionInfo.timeMemLim.memoryLimit);
}

//--------------------------------------------------------------------------------

data::DatabaseQuery::UserNames
data::DatabaseQuery::getUsersByDeleteFlag(bool aIsDeleted,
                                          const std::vector<std::string>& aMask,
                                          bool aSwitchMask)
{
    data::DatabaseQuery::UserNames result = getUsers(aMask, aSwitchMask);

    std::vector<int> turnedOffNames;
    for (auto& i : result)
    {
        if (i.second.find(TURN_OFF_STR) != std::string::npos ^ aIsDeleted)
        {
            turnedOffNames.emplace_back(i.first);
        }
    }

    for (auto& i : turnedOffNames)
    {
        result.erase(i);
    }

    return result;
}

//--------------------------------------------------------------------------------
