#include "core.hpp"

#include <fstream>
#include <functional>
#include <map>
#include <set>

#include "domain/error_message.hpp"
#include "domain/file_reader.hpp"

#define INPUT_FIR_NAME_FILE "firstName.name"
#define INPUT_SEC_NAME_FILE "secondName.name"
#define INPUT_NAMES_FILE    "name.txt"

//--------------------------------------------------------------------------------

core::Core::Core(const std::string& aDatabasePath) noexcept
    : mDBQ(aDatabasePath)
{
    WRITE_LOG("Creating_core");
}

//--------------------------------------------------------------------------------

struct Winner
{
    std::wstring name;
    int points = 0;
    std::map<int, std::string> answers;
    bool operator<(const Winner& other) const
    {
        if (points == other.points) return name > other.name;
        else return points < other.points;
    }
};

void
core::Core::run(const std::vector<std::string>& argv) noexcept
{
    mCommand.process(argv);

    std::map<Command::Type, std::function<void()>> methodMap{
        {Command::Type::GET_USERS,           [&]() { printUsers(); }       },
        {Command::Type::GET_ACTIVE_USERS,    [&]() { printUsers(); }       },
        {Command::Type::GET_DELETED_USERS,   [&]() { printUsers(); }       },
        {Command::Type::RENAME_USERS,        [&]() { renameUsers(); }      },
        {Command::Type::GENERATE_NAMES,      [&]() { generateNames(); }    },
        {Command::Type::GENERATE_ROBO_NAMES, [&]() { generateRoboNames(); }},
        {Command::Type::EVALUATE,            [&]() { getResults(); }       }
    };

    for (auto& i : methodMap)
    {
        if (mCommand.getCommand() == i.first) i.second;
    }
}

void
eval::Core::printUsers() noexcept
{
    std::map<int, std::string> names;
    bool invert = int(mFlags) & int(CommandFlag::INVERT_MASK);
    if (mCommand == Command::GET_USERS)
    {
        bool allUsers = int(mFlags) & int(CommandFlag::ALL_USERS);

        if (allUsers)
        {
            names = mDBQ.getUsers(mArgs[int(CommandFlag::HAS_MASK)], invert);
        }
        else
        {
            names =
                mDBQ.getActiveUsers(mArgs[int(CommandFlag::HAS_MASK)], invert);
        }
    }
    else if (mCommand == Command::GET_ACTIVE_USERS)
    {
        names = mDBQ.getUsers(mArgs[int(CommandFlag::HAS_MASK)], invert);
    }
    else if (mCommand == Command::GET_DELETED_USERS)
    {
        names = mDBQ.getUsers(mArgs[int(CommandFlag::HAS_MASK)], invert);
    }

    printf("%d\n", names.size());
    if (int(mFlags) | int(CommandFlag::PRINT_LOG))
    {
        auto namesAndPass = mDBQ.getPasswords(names);
        for (auto& i : namesAndPass)
        {
            printf("%s\t\t%s\n", i.first.c_str(), i.second.c_str());
        }
    }
}

void
eval::Core::renameUsers() noexcept
{
    std::map<int, std::string> names;
    bool invert   = int(mFlags) & int(CommandFlag::INVERT_MASK);
    bool allUsers = int(mFlags) & int(CommandFlag::ALL_USERS);

    if (allUsers)
    {
        names = mDBQ.getUsers(mArgs[int(CommandFlag::HAS_MASK)], invert);
    }
    else
    {
        names = mDBQ.getActiveUsers(mArgs[int(CommandFlag::HAS_MASK)], invert);
    }

    std::vector<std::string> newNames;
    std::string s;
    std::ifstream file(NEW_NAMES_FILENAME);
    while (file >> s)
        newNames.emplace_back(std::move(s));

    printf("%d\n", std::min(names.size(), newNames.size()));
    mDBQ.rename(names, std::move(newNames),
                int(mFlags) & int(CommandFlag::TEST));
}

void
core::Core::generateNames() noexcept
{

    if (!mCommand.getArgCell(0, 0).has_value())
    {
        printf("ERROR: must specify the number of users!\n");
        return;
    }
    int count = std::stoi(mCommand.getArgCell(0, 0).value());
    printf("Try to generate %d users.\n", count);

    std::ofstream output(OUTPUT_NAMES_FILE);

    auto firstNames = dom::FileReader::getAllStrings(INPUT_FIR_NAME_FILE);
    auto secondName = dom::FileReader::getAllStrings(INPUT_SEC_NAME_FILE);

    std::set<std::string> names;
    while (names.size() < count)
    {
        names.insert(firstNames[rand() % firstNames.size()] +
                     secondName[rand() % secondName.size()]);
    }

    std::string pref = "";
    if (mCommand.getFlags() & uint32_t(Command::Flag::HAS_PREFIX))
    {
        pref = mCommand.getArgCell();
    }
    for (auto& i : names)
    {
        if (maskFlag)
        {
            output << mCommand.getArgs()
                          .find(int(Command::Flag::HAS_PREFIX))
                          ->second[0]
                   << "-";
        }
        printf("%s%s\n", pref, i.c_str());
    }
}

void
eval::Core::generateRoboNames() noexcept
{
    if (mArgs[0].size() < 0)
    {
        printf("ERROR: must specify the number of users!\n");
        exit(0);
    }
    int count = std::stoi(mArgs[0][0]);
    printf("Try to generate %d users.\n", count);

    std::set<std::string> names;
    while (names.size() < count)
    {
        std::string name;
        for (int i = 0; i < 12; ++i)
            name.push_back(rand() % ('z' - 'a') + 'a');
        names.insert(std::move(name));
    }

    std::ofstream output(NEW_NAMES_FILENAME);
    bool maskFlag = int(mFlags) & int(CommandFlag::HAS_MASK);
    for (auto& i : names)
    {
        if (maskFlag)
        {
            output << mArgs[int(CommandFlag::HAS_MASK)][0];
        }
        output << i << "\n";
    }
}

#include "domain/cyrillic.hpp"
void
eval::Core::getResults() noexcept
{
    if (mArgs.count(0) == 0)
    {
        printf("ERROR: must specify the competition info file!\n");
        exit(0);
    }

    std::ifstream compInfoFile(mArgs[0][0]);
    std::string aCompetitionName;
    while (std::getline(compInfoFile, aCompetitionName))
    {
        if (!aCompetitionName.size())
            std::getline(compInfoFile, aCompetitionName);
        std::wcout << dom::Cyrilic::global.toWString(aCompetitionName) << L"\n";

        auto compInfo     = mDBQ.getCompetitionInfo(aCompetitionName);
        auto questionsIDs = mDBQ.getQuestionNumbers(compInfo.id);

        auto groupsIDs = mDBQ.getGroupIDs(compInfo.id);
        auto userIDs   = mDBQ.getUserIDs(groupsIDs);
        auto userNames = mDBQ.getUserNames(userIDs);

        std::map<int, Question> questions;
        auto questionsTemp = mDBQ.getQuestions(questionsIDs);
        for (auto& i : questionsTemp)
        {
            questions[i.first].ans.push_back(i.second);
            // questions[i.first].id = i.first;
        }

        auto questionNames = mDBQ.getQuestionNames(questionsIDs);
        for (auto& i : questionNames)
        {
            questions[i.second].name = i.first;
        }

        {
            std::string temp;
            int id = 0;
            while (compInfoFile >> temp)
            {
                if (temp == "ANS:")
                {
                    std::getline(compInfoFile, temp);
                    std::wstring ans = dom::Cyrilic::global.toWString(temp);
                    dom::Cyrilic::global.standardProcedure(ans);
                    questions[id].ans.clear();
                    questions[id].ans.push_back(ans);
                }
                else if (temp == "ADD_ANS:")
                {
                    std::getline(compInfoFile, temp);
                    std::wstring ans = dom::Cyrilic::global.toWString(temp);
                    dom::Cyrilic::global.standardProcedure(ans);
                    questions[id].ans.push_back(ans);
                }
                else if (temp == "TYPE:")
                {
                    std::getline(compInfoFile, temp);
                    if (temp == " Standart")
                    {
                        questions[id].type = QuestionType::Standart;
                    }
                    else if (temp == " Multiple")
                    {
                        questions[id].type = QuestionType::Multiple;
                    }
                    else if (temp == " MultipleStrong")
                    {
                        questions[id].type = QuestionType::MultipleStrong;
                    }
                    else if (temp == " Complex")
                    {
                        questions[id].type = QuestionType::Complex;
                    }
                }
                else if (temp == "WEIGHT:")
                {
                    std::getline(compInfoFile, temp);
                    questions[id].weight = std::stod(temp);
                }
                else if (temp == "DELETE")
                {
                    questions.erase(id);
                }
                else if (temp == "END")
                {
                    break;
                }
                else
                {
                    id = std::stoi(temp);
                }
            }
        }

        // {
        //     std::vector<int> nums;
        //     for(auto& q : questions)
        //     {
        //         nums.emplace_back(q.second.id);
        //     }
        //     auto questionNames = mDBQ.getQuestionNames(nums);
        //     for(auto& i : questionNames)
        //     {
        //         questions[i.second].name = i.first;
        //     }
        // }
        // auto questionNames = mDBQ.getQuestionNames(questionsIDs);
        // for(auto& i : questionNames)
        // {
        //     if (questions.contains(i.second)) questions[i.second].name =
        //     i.first;
        // }

        std::map<std::wstring, int> questionMap;
        for (auto& s : questions)
        {
            questionMap[s.second.name] = s.first;
        }

        if (int(mFlags) & int(CommandFlag::PRINT_LOG))
        {
            int cnt = 0;
            for (auto& i : questionMap)
            {
                auto it = questions.find(i.second);
                std::wprintf(L"%2d%4d%s %4.1f %d = ", ++cnt, it->first, L": ",
                             it->second.weight, int(it->second.type));

                std::wstring res;
                for (int j = 0; j < it->second.ans.size(); ++j)
                {
                    if (j > 0) res += L" || ";
                    res += it->second.ans[j];
                }

                // std::wprintf(L"%hs (%ls)\n", it->second.name, res);
                // std::wcout << res << L" (" + it->second.name + L")\n";
                std::wcout << L"(" + it->second.name + L") = " << res << L'\n';

                // auto it = questions.find(i.second);
                // std::wcout << std::to_wstring(it->first) << L": " <<
                //     std::to_wstring(it->second.weight) << L" = ";

                // for(int j = 0; j < it->second.ans.size(); ++j)
                // {
                //     if (j > 0) std::wcout << L" || ";
                //     std::wcout << it->second.ans[j];
                // }
                // std::wcout <<  L" (" << it->second.name << L")\n";
            }

            // std::wcout << L"\nAnswers:\n";
            // for(auto& i : questions)
            // {
            //     std::wcout << i.first << L" ";
            //     for(int j = 0; j < i.second.size(); ++j)
            //     {
            //         if (j > 0) std::wcout << L" || ";
            //         std::wcout << i.second[j];
            //     }
            //     std::wcout << L"\n";
            // }
            // std::wcout << L"\n";

            // std::wcout << L"Type:\n";
            // for(auto& i : questionTypes)
            // {
            //     std::wcout << i.first << L" " << int(i.second) << L"\n";
            // }
            // std::wcout << L"\n";

            // std::wcout << L"Weight:\n";
            // for(auto& i : questionWeight)
            // {
            //     std::wcout << i.first << L" " << i.second << L"\n";
            // }
            // std::wcout << L"\n";
        }
        // continue;
        for (auto& uID : userIDs)
        {
            auto userAns = mDBQ.getUserAnswers(questionsIDs, uID);
            std::wcout << userNames[uID] << L" ";
            double s = 0;
            for (auto qID : questionMap)
            {
                auto& q = *questions.find(qID.second);
                if (q.first == 473)
                {
                    int yy = 0;
                    ++yy;
                }

                if (int(mFlags) & int(CommandFlag::PRINT_LOG))
                {
                    std::wcout << L"\n"
                               << q.second.ans[0] << L" === "
                               << userAns[q.first];
                }
                int correctCount = 0;
                if (q.second.type == QuestionType::Standart)
                {
                    for (auto& ans : q.second.ans)
                    {
                        if (ans == userAns[q.first])
                        {
                            correctCount = 1;
                            break;
                        }
                    }
                }
                else if (q.second.type == QuestionType::Multiple ||
                         q.second.type == QuestionType::Complex)
                {
                    dom::Cyrilic::global.destroyWhiteSpaces(userAns[q.first],
                                                            true);
                    for (auto& ans : q.second.ans)
                    {
                        int cnt = 0;
                        for (int k = 0; k < ans.size(); ++k)
                        {
                            if (userAns[q.first].find(ans[k]) !=
                                    std::wstring::npos &&
                                ans[k] != L' ')
                            {
                                ++cnt;
                            }
                        }
                        if (cnt > 1 && ans.size() != userAns[q.first].size())
                        {
                            --cnt;
                        }
                        correctCount = std::max(correctCount, cnt);
                        cnt          = 0;
                    }
                }
                else if (q.second.type == QuestionType::MultipleStrong)
                {
                    dom::Cyrilic::global.destroyWhiteSpaces(userAns[q.first],
                                                            true);
                    for (auto& ans : q.second.ans)
                    {
                        int cnt = 0;
                        if (ans.size() != userAns[q.first].size())
                        {
                            correctCount = 0;
                            break;
                        }
                        for (int k = 0; k < ans.size(); ++k)
                        {
                            if (userAns[q.first].size() <= k) break;
                            if (ans[k] == userAns[q.first][k] && ans[k] != L' ')
                            {
                                ++cnt;
                            }
                        }
                        correctCount = std::max(correctCount, cnt);
                        cnt          = 0;
                    }
                }

                double old_s = s;
                if (correctCount)
                {
                    if (q.second.type != QuestionType::Complex)
                    {
                        s += q.second.weight * correctCount;
                    }
                    else
                    {
                        if (q.first == 463)
                        {
                            if (correctCount > 3 && correctCount < 6) s += 0.5;
                            else if (correctCount >= 6) s += 1;
                        }
                        if (q.first == 468)
                        {
                            if (correctCount < 6)
                                ;
                            else if (correctCount < 7) s += 1;
                            else if (correctCount < 13) s += 2;
                            else s += 3;
                        }
                        if (q.first == 473)
                        {
                            if (correctCount < 1)
                                ;
                            else if (correctCount < 2) s += 0.5;
                            else s += 1;
                        }
                    }
                }

                if (int(mFlags) & int(CommandFlag::PRINT_LOG))
                {
                    if (correctCount)
                    {
                        std::wcout << L" + " << s - old_s;
                    }
                    else
                    {
                        std::wcout << L" - " << s - old_s;
                    }
                }
                else
                {
                    if (userAns[q.first].size() == 0) std::wcout << L"\t-";
                    else std::wcout << L"\t" << s - old_s;
                }
            }
            if (int(mFlags) & int(CommandFlag::PRINT_LOG)) std::wcout << L"\n";
            else std::wcout << L"\t=\t";
            std::wcout << s << L"\n";
            if (int(mFlags) & int(CommandFlag::PRINT_LOG))
            {
                std::wcout << L"\n";
            }
        }
    }
}