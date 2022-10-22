#include "database/database_query.hpp"

#include <vector>

//--------------------------------------------------------------------------------

#include "domain/error_message.hpp"

#define TURN_OFF_STR "-----"

//--------------------------------------------------------------------------------

data::DatabaseQuery::DatabaseQuery (const std::string& aDatabasePath) noexcept :
    mDatabase                   (aDatabasePath),
    mReservedStatementNumber    (0),
    mTestNum                    (1),
    mTestAreOver                (false)
{
    WRITE_LOG("Creating_database_quare");
}

//--------------------------------------------------------------------------------

void
data::DatabaseQuery::rename
(
    const std::map<int, std::string>& aOldNames, 
    std::vector<std::string>&& aNewNames,
    bool aOnlyTest
) noexcept
{
    for(auto& i : aOldNames)
    {
        if (aNewNames.size() == 0) break;

        if (aOnlyTest)
        {
            printf("%s   ----->   %s\n", i.second.c_str(), aNewNames.back().c_str());
        }
        else
        {
            mDatabase.update("core_users", "username = \'" + aNewNames.back() + "\'", 
                "id = " + std::to_string(i.first));
            mDatabase.step();
            mDatabase.closeStatment();
        }
        aNewNames.pop_back();
    }
}

void
data::DatabaseQuery::turnOff(std::vector<std::string> nameTemplate) noexcept
{
    mDatabase.select("core_users", "id, username, is_superuser, is_teacher, is_staff");
    std::vector<int> ids;
    std::vector<std::string> names;
    while(true)
    {

        mDatabase.step();
        auto num = mDatabase.getIntFromRow(0);

        //if (!num.has_value())
        if (num == -1)
        {
            break;
        }

        auto name = mDatabase.getTextFromRow(1).value();

        bool flag = mDatabase.getBool(2);
        if (flag == 1) 
        {
            continue;
        }
        flag = mDatabase.getBool(3);
        if (flag == 1) 
        {
            continue;
        }
        flag = mDatabase.getBool(4);
        if (flag == 1) 
        {
            continue;
        }

        std::string nameS = name.getString();
        flag = false;
        for(auto& i : nameTemplate)
        {
            flag |= nameS.find(i) != -1;
        }
        if (!flag == false) continue;


        if (name == "pashs'" || name == "AlphaPrimus" ||
            name == "ITMO-Student" || name == "Ivan") continue;

        ids.push_back(num);
        names.emplace_back(std::move(name));
    }
    mDatabase.closeStatment();

    std::cout << ids.size() << "\n";

    for(int i = 0; i < ids.size(); ++i)
    {
        std::string sss = "-----";
        if (names[i].find(sss) != -1) sss = "";
        std::string newUserName = sss + names[i];

        mDatabase.update("core_users", "username = \'" + newUserName + "\'", 
            "id = " + std::to_string(ids[i]));
        mDatabase.step();
        mDatabase.closeStatment();

        //std::cout << newUserName << std::endl;
    }
}
#include <algorithm>
void
data::DatabaseQuery::turnOn() noexcept
{
    mDatabase.select("core_users", "id, username, is_superuser, is_teacher, is_staff");
    std::vector<int> ids;
    std::vector<std::string> names;
    while(true)
    {

        mDatabase.step();
        auto num = mDatabase.getIntFromRow(0);

        //if (!num.has_value())
        if (num == -1)
        {
            break;
        }

        auto name = mDatabase.getTextFromRow(1).value();

        bool flag = mDatabase.getBool(2);
        if (flag == 1) 
        {
            continue;
        }
        flag = mDatabase.getBool(3);
        if (flag == 1) 
        {
            continue;
        }
        flag = mDatabase.getBool(4);
        if (flag == 1) 
        {
            continue;
        }

        std::string nameS = name.getString();
        flag = nameS.find("----") != -1;
        if (flag == false) continue;


        if (name == "pashs'" || name == "AlphaPrimus" ||
            name == "ITMO-Student" || name == "Ivan") continue;

        ids.push_back(num);
        names.emplace_back(std::move(name));
    }
    mDatabase.closeStatment();

    std::cout << ids.size() << "\n";

    for(int i = 0; i < ids.size(); ++i)
    {
        //std::cout << names[i] << "  ---->   ";
        reverse(names[i].begin(), names[i].end());
        names[i].resize(names[i].size() - 5);
        reverse(names[i].begin(), names[i].end());
        std::string newUserName = names[i];

        mDatabase.update("core_users", "username = \'" + newUserName + "\'", 
            "id = " + std::to_string(ids[i]));
        mDatabase.step();
        mDatabase.closeStatment();

        //std::cout << newUserName << std::endl;
    }
}
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
data::DatabaseQuery::writeResult
(
    int                 aID, 
    const std::string&  aResult,
    int                 aTime, 
    int                 aMemory
) noexcept
{
    mDatabase.closeStatment(0);

    START_LOG_BLOCK("Updating_database");
    WRITE_LOG("Updating...");

    mDatabase.update("core_solutions", "result = '" + aResult + "'" +
        ", time = " + std::to_string(aTime) +
        ", memory = " + std::to_string(aMemory), "id = " + std::to_string(aID));
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
        //if (mDatabase.step() != SQLITE_OK) break; TODO: fixing that
        mDatabase.step(mReservedStatementNumber);
        input = mDatabase.getTextFromRow(0, mReservedStatementNumber);
        output = mDatabase.getTextFromRow(1, mReservedStatementNumber);
        tempNum = mTestNum++;

        if (!input.has_value())
        {
            mTestAreOver = true;
        }
    }
    mTestMutex.unlock();

    if (input.has_value())
    {
        data.input = std::move(input.value());
        data.output = std::move(output.value());
        data.testNum = tempNum;
        result = std::make_optional<TestData>(std::move(data));
    }

    return result;
}

//--------------------------------------------------------------------------------

void
data::DatabaseQuery::prepareTestsStatement(uint64_t aProblemID) noexcept
{
    START_LOG_BLOCK("Prepare_geting_test_from_database");
    mDatabase.select("core_test", "input, output", "contest_id = " +
        std::to_string(aProblemID), mReservedStatementNumber);
    END_LOG_BLOCK("I'm_ready");
}
//--------------------------------------------------------------------------------

data::DatabaseQuery::CompetitionData
data::DatabaseQuery::getCompetitionInfo(const std::string& aCompetitionName) noexcept
{
    CompetitionData result;
    //START_LOG_BLOCK("Prepare_geting_test_from_database");

    mDatabase.select("core_competitions", "id, start_time, end_time", "name = " +
        "\'"s + aCompetitionName + "\'");
    mDatabase.step();
    result.id = mDatabase.getInt64FromRow(0);
    mDatabase.closeStatment();
    //TODO: time
    //result.startTime = mDatabase.getTextFromRow(1);

    //END_LOG_BLOCK("I'm_ready");
    return result;
}

//--------------------------------------------------------------------------------

std::vector<int>
data::DatabaseQuery::getQuestionNumbers(int aCompetitionID) noexcept
{
   // CompetitionData result;
    //START_LOG_BLOCK("Prepare_geting_test_from_database");

    mDatabase.select("core_competitions_questions", "question_id", "competitions_id = " +
        "\'"s + std::to_string(aCompetitionID) + "\'");
    
    std::vector<int> result;
    while(true)
    {

        mDatabase.step();
        auto num = mDatabase.getInt64FromRow(0);

        //if (!num.has_value())
        if (num == -1)
        {
            break;
        }
        result.push_back(num);
    }
    mDatabase.closeStatment();
    
    // std::cout << "core_competitions_questions:" << "\n";
    // for(auto i : result) std::cout << i << " ";
    // std::cout << "\n\n";

    return result;


    //core_questionans

 //   END_LOG_BLOCK("I'm_ready");
  //  return result;
}

//--------------------------------------------------------------------------------
#include <map>
#include <sstream>
#include <cwctype>

#include "domain/cyrillic.hpp"

std::map<int, std::wstring> 
data::DatabaseQuery::getQuestions(const std::vector<int>& aQuestionNumbers) noexcept
{
    std::map<int, std::wstring>  result;

    for(auto questionNum : aQuestionNumbers)
    {
        mDatabase.select("core_question", "question", "id = " +
            "\'"s + std::to_string(questionNum) + "\'");
        
        while(true)
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
                for(auto& i : s)
                {
                    if (i == L' ')
                        i = L'_';
                }
                for(auto& i : s)
                {
                    if (i == L'\"' || i == L'\\' ||
                        i == L'[' || i == L']' ||
                        i == L':' || i == L',')
                        i = L' ';
                }
                std::reverse(s.begin(), s.end());

                std::wstringstream sss;
                sss << s;
                for(int i = 0; sss >> s; ++i)
                {
                    sss >> s;
                    sss >> s;
                    if (s == L"true") ss += std::to_wstring(i) + L" ";
                }

                //ss = s;
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
    
    // std::cout << "core_question:" << "\n";
    // for(auto i : result) std::cout << i.first << " - " <<
    // i.second << "\n";
    // std::cout << "size: " << result.size() << "\n\n";

    return result;


    //core_questionans

 //   END_LOG_BLOCK("I'm_ready");
  //  return result;
}

//--------------------------------------------------------------------------------

std::vector<int>
data::DatabaseQuery::getGroupIDs(int aCompetitionID) noexcept
{
   // CompetitionData result;
    //START_LOG_BLOCK("Prepare_geting_test_from_database");

    mDatabase.select("core_studentgroup_competitions", "studentgroup_id", 
    "competitions_id = " +
        "\'"s + std::to_string(aCompetitionID) + "\'");
    
    std::vector<int> result;
    while(true)
    {

        mDatabase.step();
        auto num = mDatabase.getInt64FromRow(0);

        //if (!num.has_value())
        if (num == -1)
        {
            mDatabase.closeStatment();
            break;
        }
        result.push_back(num);
    }
    mDatabase.closeStatment();
    
    // std::cout << "core_studentgroup_competitions:" << "\n";
    // for(auto i : result) std::cout << i << " ";
    // std::cout << "\n\n";

    return result;


    //core_questionans

 //   END_LOG_BLOCK("I'm_ready");
  //  return result;
}

std::map<std::wstring, int> 
data::DatabaseQuery::getQuestionNames(const std::vector<int>& aIDs) noexcept
{
    std::map<std::wstring, int> result;
    
    for(auto& id : aIDs)
    {
        mDatabase.select("core_question", "name", 
            "id = " + std::to_string(id));
        mDatabase.step();
        result[mDatabase.getText16FromRow(0).value()] = id;
        mDatabase.closeStatment();
    }

    return result;


    //core_questionans

 //   END_LOG_BLOCK("I'm_ready");
  //  return result;
}

//--------------------------------------------------------------------------------

std::vector<int>
data::DatabaseQuery::getUserIDs(const std::vector<int>& aGroups) noexcept
{
   // CompetitionData result;
    //START_LOG_BLOCK("Prepare_geting_test_from_database");

    std::vector<int> result;

    for(auto groupNum : aGroups)
    {
        mDatabase.select("core_studentgroup_users", "users_id", "studentgroup_id = " +
            "\'"s + std::to_string(groupNum) + "\'");
        
        while(true)
        {

            mDatabase.step();
            auto num = mDatabase.getInt64FromRow(0);

            //if (!num.has_value())
            if (num == -1)
            {
                mDatabase.closeStatment();
                break;
            }
            result.push_back(num);
        }
        mDatabase.closeStatment();
    }

    // std::cout << "core_studentgroup_users:" << "\n";
    // for(auto i : result) std::cout << i << " ";
    // std::cout << "\n\n";
    // std::cout << "size: " << result.size() << "\n\n";

    return result;


    //core_questionans

 //   END_LOG_BLOCK("I'm_ready");
  //  return result;
}


std::map<int, std::map<int, std::wstring>>
data::DatabaseQuery::getUserAnswers(const std::vector<int>& aUserIDs,
    const std::vector<int>& aQuestionNumbers) noexcept
{
   // CompetitionData result;
    //START_LOG_BLOCK("Prepare_geting_test_from_database");

    std::map<int, std::map<int, std::wstring>> result;

    for(auto question : aQuestionNumbers)
    {
        for(auto user : aUserIDs)
        {
            mDatabase.select("core_questionans", "ans", "question_id = " +
                std::to_string(question) + " AND user_id = " +
                std::to_string(user) );
            
            while(true)
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


    //core_questionans

 //   END_LOG_BLOCK("I'm_ready");
  //  return result;
}

std::map<int, std::wstring>
data::DatabaseQuery::getUserAnswers
(
    const std::vector<int>& aQuaestionIDs,
    int aUserId
) noexcept
{
    std::map<int, std::wstring> result;
    for(auto question : aQuaestionIDs)
    {

        mDatabase.select("core_questionans", "ans", "question_id = " +
            std::to_string(question) + " AND user_id = " +
            std::to_string(aUserId));
        
        while(true)
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
    return result;
}

std::map<int, std::wstring>
data::DatabaseQuery::getUserNames(const std::vector<int>& aUserIDs) noexcept
{
   // CompetitionData result;
    //START_LOG_BLOCK("Prepare_geting_test_from_database");

    std::map<int, std::wstring> result;


    for(auto user : aUserIDs)
    {
        mDatabase.select("core_users", "username", "id = " +
            std::to_string(user));
        

        mDatabase.step();
        auto name = mDatabase.getText16FromRow(0);

        if (!name.has_value())
        {
            mDatabase.closeStatment();
            break;
        }
        result[user] = name.value();
        mDatabase.closeStatment();
    }

    // std::cout << "core_users:" << "\n";
    // for(auto i : result) std::cout << i.first << " - " <<
    //     i.second << "\n";
    // std::cout << "size: " << result.size() << "\n\n";

    return result;


    //core_questionans

 //   END_LOG_BLOCK("I'm_ready");
  //  return result;
}

//--------------------------------------------------------------------------------

bool 
data::DatabaseQuery::isUserHasBron(const std::string& aName)
{
    return 
        aName == "pashs" || 
        aName == "ITMO-Student" ||
        aName == "AlphaPrimus" || 
        aName == "Ivan" || 
        aName == "Ivanus-Primus";
}

//--------------------------------------------------------------------------------

data::DatabaseQuery::UserNames
data::DatabaseQuery::getUsers
(
    const std::vector<std::string>& aMask, 
    bool aSwitchMask
)
{
    data::DatabaseQuery::UserNames result;

    mDatabase.select("core_users", "id, username, is_superuser, "
    "is_teacher, is_staff");
    while(true)
    {
        mDatabase.step();
        if (!mDatabase.hasData()) break;

        if (mDatabase.getBool(2)) continue;
        if (mDatabase.getBool(3)) continue;
        if (mDatabase.getBool(4)) continue;

        auto id = mDatabase.getIntFromRow(0);
        auto name = mDatabase.getTextFromRow(1).value().getString();
        
        if (isUserHasBron(name)) continue;
        
        bool flag = false;
        for(auto& i : aMask)
        {
            flag |= name.find(i) != std::string::npos;
        }
        if (flag ^ (!aSwitchMask)) continue;

        result[id] = name;
    }
    mDatabase.closeStatment();

    return result;
}

std::map<std::string, std::string>
data::DatabaseQuery::getPasswords(UserNames aUsers)
{
    std::map<std::string, std::string> result;

    for(auto& i : aUsers)
    {
        mDatabase.select("core_passwords", "password", 
            "user_id = " + std::to_string(i.first));

        mDatabase.step();
        result[i.second] = mDatabase.getTextFromRow(0).value().getString();
        mDatabase.closeStatment();
    }

    return result;
}

//--------------------------------------------------------------------------------

data::DatabaseQuery::UserNames
data::DatabaseQuery::getActiveUsers
(
    const std::vector<std::string>& aMask, 
    bool aSwitchMask
)
{
    return getUsersByDeleteFlag(false, aMask, aSwitchMask);
}

//--------------------------------------------------------------------------------

data::DatabaseQuery::UserNames
data::DatabaseQuery::getDeletedUsers
(
    const std::vector<std::string>& aMask, 
    bool aSwitchMask
)
{
    return getUsersByDeleteFlag(true, aMask, aSwitchMask);
}

//--------------------------------------------------------------------------------

void
data::DatabaseQuery::getParticipantInfo(SubmissionInfo& aSubmissionInfo) noexcept
{
    START_LOG_BLOCK("Geting_ID_and_name_from_database");

    mDatabase.select("core_solutions", "file_name, contest_id", "id = " + 
        std::to_string(aSubmissionInfo.ID));
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
    aSubmissionInfo.timeMemLim = { uint64_t(mDatabase.getInt64FromRow(0)), 
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
data::DatabaseQuery::getUsersByDeleteFlag
(
    bool aIsDeleted, 
    const std::vector<std::string>& aMask, 
    bool aSwitchMask
)
{
    data::DatabaseQuery::UserNames result = getUsers(aMask, aSwitchMask);
   
    std::vector<int> turnedOffNames;
    for(auto& i : result)
    {
        if (i.second.find(TURN_OFF_STR) != std::string::npos ^ aIsDeleted)
        {
            turnedOffNames.emplace_back(i.first);
        }
    }

    for(auto& i : turnedOffNames)
    {
        result.erase(i);
    }

    return result;
}

//--------------------------------------------------------------------------------
