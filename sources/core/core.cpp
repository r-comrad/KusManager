#include "core.hpp"
#define MASK_FILENAME "mask.name"
#define NEW_NAMES_FILENAME "new_name.name"

//--------------------------------------------------------------------------------

eval::Core::Core(const std::string& aDatabasePath) noexcept :
    mDBQ            (aDatabasePath)
{
    WRITE_LOG("Creating_core");
}



//--------------------------------------------------------------------------------

struct Winner
{
    std::wstring name;
    int points = 0;
    std::map<int, std::string> answers;
    bool operator< (const Winner& other)const
    {
        if (points == other.points) return name > other.name;
        else return points < other.points;
    }
};
#include <fstream>
#include <set>

void
eval::Core::run(int argc, char* argv[]) noexcept
{
    parseCommandLine(argc, argv);

    if (mCommand == Command::GET_USERS || 
        mCommand == Command::GET_ACTIVE_USERS ||
        mCommand == Command::GET_DELETED_USERS)
    {
        printUsers();
    }    
    else if (mCommand == Command::RENAME_USERS)
    {
        renameUsers();
    }    
    else if (mCommand == Command::GENERATE_NAMES)
    {
        generateNames();
    }
    else if (mCommand == Command::GENERATE_ROBO_NAMES)
    {
        generateRoboNames();
    }
    else if (mCommand == Command::EVALUATE)
    {
        getResults();
    }
}

// void
// eval::Core::run() noexcept
// {
//     std::ifstream namesFile("out.name");
//     std::vector<std::string> names;
//     std::string inp;
//     while(namesFile >> inp) names.push_back(inp);

//     mDBQ.rename(names);
// }

void
eval::Core::makeNames() noexcept
{
    std::ofstream output("out.name");

    std::string inp;

    std::ifstream file("firstName.txt");
    std::vector<std::string> firstNames;
    while(file >> inp) firstNames.push_back(inp);

    file.close();
    file.open("secondName.txt");
    std::vector<std::string> secondName;
    while(file >> inp) secondName.push_back(inp);

    file.close();
    file.open("names.txt");
    while(file >> inp)
    {
        std::string add;
        file >> add;
        std::vector<int> size(7);
        for(auto& i : size) 
        {
            file >> i;
            if (i)
            {
                if (i / 2 > 50) i += 50;
                else i += 25;
            }
        }

        if (inp == "-s")
        {
            for(int i = 0; i < size.size(); ++i)
            {
                std::set<std::string> names;
                while(names.size() < size[i])
                {
                    names.insert(firstNames[rand()%firstNames.size()] + "-" +  secondName[rand()%secondName.size()]);
                }   

                for(auto& j : names) 
                {
                    std::string full_name = add + "-" + std::to_string(i + 5) + "-" + j + "\n";
                    output << full_name;
                }
                output << "\n";
            }
        }
        else if (inp == "-n")
        {
            for(int i = 0; i < size.size(); ++i)
            {
                for(auto& i : add) i = std::tolower(i);
                std::set<std::string> names;
                while(names.size() < size[i])
                {
                    std::string temp;
                    temp.reserve(12);
                    while(temp.size() < 12)
                    {
                        temp.push_back(rand() % ('z' - 'a') + 'a');
                    }
                    names.insert(temp);
                }   
                for(auto& j : names) 
                {
                    std::string full_name = add + std::to_string(i + 5) + j + "\n";
                    output << full_name;
                }
                output << "\n";
            }
        }

        output << "\n";
    }
}


void
eval::Core::parseCommandLine(int argc, char* argv[]) noexcept
{
    mCommand = Command::NUN;
    mFlags = CommandFlag::NUN;

    if(strcmp(argv[1], "getUsers") == 0 || strcmp(argv[1], "getAllUsers") == 0)
    {
        mCommand = Command::GET_USERS;
    }
    else if(strcmp(argv[1], "getActive") == 0 || strcmp(argv[1], "getActiveUsers") == 0)
    {
        mCommand = Command::GET_ACTIVE_USERS;
    }
    else if(strcmp(argv[1], "getDeleted") == 0 || strcmp(argv[1], "getDeletedUsers") == 0)
    {
        mCommand = Command::GET_DELETED_USERS;
    }
    else if(strcmp(argv[1], "turnOffUsers") == 0)
    {
        mCommand = Command::DELETE_USERS;
    }
    else if(strcmp(argv[1], "turnOnUsers") == 0)
    {
        mCommand = Command::RESURRECT_USERS;
    }    
    else if(strcmp(argv[1], "rename") == 0)
    {
        mCommand = Command::RENAME_USERS;
    }    
    else if(strcmp(argv[1], "generateNames") == 0 || strcmp(argv[1], "generate") == 0)
    {
        mCommand = Command::GENERATE_NAMES;
    }
    else if(strcmp(argv[1], "generateRoboNames") == 0 || strcmp(argv[1], "generateRobo") == 0)
    {
        mCommand = Command::GENERATE_ROBO_NAMES;
    }
    else if(strcmp(argv[1], "getResults") == 0 || strcmp(argv[1], "evaluate") == 0)
    {
        mCommand = Command::EVALUATE;
    }

    int last = 0;
    for(int i = 2; i < argc; ++i)
    {
        if (strcmp(argv[i], "-test") == 0 || strcmp(argv[i], "-t") == 0)
        {
            mFlags = CommandFlag(int(mFlags) | int(CommandFlag::TEST));
            last = int(CommandFlag::TEST);
        }
        else if (strcmp(argv[i], "-print") == 0 || strcmp(argv[i], "-p") == 0)
        {
            mFlags = CommandFlag(int(mFlags) | int(CommandFlag::PRINT_LOG));
            last = int(CommandFlag::PRINT_LOG);
        }        
        else if (strcmp(argv[i], "-mask") == 0 || strcmp(argv[i], "-m") == 0)
        {
            mFlags = CommandFlag(int(mFlags) | int(CommandFlag::HAS_MASK));
            last = int(CommandFlag::HAS_MASK);
        }
        else if (strcmp(argv[i], "-maskFile") == 0 || strcmp(argv[i], "-mf") == 0)
        {
            mFlags = CommandFlag(int(mFlags) | int(CommandFlag::HAS_MASK));
            mFlags = CommandFlag(int(mFlags) | int(CommandFlag::HAS_MASK_FILE));
            last = int(CommandFlag::HAS_MASK);
        }
        else if (strcmp(argv[i], "-invert") == 0 || strcmp(argv[i], "-i") == 0)
        {
            mFlags = CommandFlag(int(mFlags) | int(CommandFlag::INVERT_MASK));
            last = int(CommandFlag::INVERT_MASK);
        }
        else if (strcmp(argv[i], "-all") == 0 || strcmp(argv[i], "-a") == 0)
        {
            mFlags = CommandFlag(int(mFlags) | int(CommandFlag::ALL_USERS));
            last = int(CommandFlag::ALL_USERS);
        }
        else if (argv[i][0] == '-')
        {
            continue;;
        }
        else
        {
            mArgs[last].push_back(std::string(argv[i]));
        }
    }

    if (int(mFlags) & int(CommandFlag::HAS_MASK_FILE))
    {
        std::ifstream file(MASK_FILENAME);
        std::string s;
        while (file >> s) 
        {
            mArgs[int(CommandFlag::HAS_MASK)].emplace_back(std::move(s));
        }
        file.close();
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
            names = mDBQ.getActiveUsers(mArgs[int(CommandFlag::HAS_MASK)], invert);
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
        for(auto& i : namesAndPass) 
        {
            printf("%s\t\t%s\n", i.first.c_str(), i.second.c_str());
        }
    }
}

void
eval::Core::renameUsers() noexcept
{
    std::map<int, std::string> names;
    bool invert = int(mFlags) & int(CommandFlag::INVERT_MASK);
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
    while(file >> s) newNames.emplace_back(std::move(s));

    printf("%d\n", std::min(names.size(), newNames.size()));
    mDBQ.rename
    (
        names,
        std::move(newNames),
        int(mFlags) & int(CommandFlag::TEST)
    );
}

void
eval::Core::generateNames() noexcept
{
    if (mArgs[0].size() < 0)
    {
        printf("ERROR: must specify the number of users!\n");
        exit(0);
    }
    int count = std::stoi(mArgs[0][0]);
    printf("Try to generate %d users.\n", count);

    std::string inp;

    std::ifstream file("firstName.txt");
    std::vector<std::string> firstNames;
    while(file >> inp) firstNames.push_back(std::move(inp));
    file.close();

    file.open("secondName.txt");
    std::vector<std::string> secondName;
    while(file >> inp) secondName.push_back(std::move(inp));
    file.close();

    std::set<std::string> names;
    while(names.size() < count)
    {
        names.insert(firstNames[rand() % firstNames.size()] + 
            secondName[rand() % secondName.size()]);
    }

    std::ofstream output(NEW_NAMES_FILENAME);
    bool maskFlag = int(mFlags) & int(CommandFlag::HAS_MASK);
    for(auto& i : names)
    {
        if (maskFlag)
        {
            output << mArgs[int(CommandFlag::HAS_MASK)][0] << "-";
        }
        output << i << "\n";
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
    while(names.size() < count)
    {
        std::string name;
        for(int i = 0; i < 12; ++i) name.push_back(rand()% ('z' - 'a') + 'a');
        names.insert(std::move(name));
    }

    std::ofstream output(NEW_NAMES_FILENAME);
    bool maskFlag = int(mFlags) & int(CommandFlag::HAS_MASK);
    for(auto& i : names)
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
    {
        std::string temp;
        getline(compInfoFile, aCompetitionName);
        //aCompetitionName = temp;
    }
    std::wcout << dom::Cyrilic::global.toWString(aCompetitionName) << L"\n";

    auto compInfo = mDBQ.getCompetitionInfo(aCompetitionName);
    auto questionsIDs = mDBQ.getQuestionNumbers(compInfo.id);


    auto groupsIDs = mDBQ.getGroupIDs(compInfo.id);
    auto userIDs = mDBQ.getUserIDs(groupsIDs);
    auto userNames = mDBQ.getUserNames(userIDs);

    // {
    //     auto questionNames = mDBQ.getQuestionNames(questionsIDs);
    //     decltype(questionNames) questionNameNumbers;
    //     std::wstring temp;
    //     for(auto& i : questionNames)
    //     {
    //         std::wstring name;
    //         name.push_back(i.first[i.first.size() - 2]);
    //         name.push_back(i.first.back());
    //         questionNameNumbers[name] = i.second;
    //     }
        
    //     while(compInfoFile >> temp)
    //     {
    //         std::wstring name = temp;
    //         compInfoFile >> temp;
    //         dom::Cyrilic::global.standardProcedure(temp);
    //         questions[questionNameNumbers[name]] = temp;
    //     }

    //     if (int(mFlags) | int(CommandFlag::PRINT_LOG)) 
    //     {
    //         for(auto& i : questions)
    //         {
    //             std::wcout << i.first << L" " << i.second << L"\n";
    //         }
    //     }
    // }

    std::map<int, std::vector<std::wstring>> questions;
    std::map<int, char> questionTypes;
    std::map<int, double> questionWeight;
    {
        auto questionsTemp = mDBQ.getQuestions(questionsIDs);
        for(auto& i : questionsTemp)
        {
            questions[i.first].push_back(i.second);
        }
    }
    for(auto& i : questions)
    {
        questionTypes[i.first] = 0;
        questionWeight[i.first] = 1;
    }

    {
        std::string temp;
        int id = 0;
        while(compInfoFile >> temp)
        {
            if (temp == "ANS:")
            {
                std::getline(compInfoFile, temp);
                std::wstring ans = dom::Cyrilic::global.toWString(temp);
                dom::Cyrilic::global.standardProcedure(ans);
                questions[id].clear();
                questions[id].push_back(ans);
            }    
            else if (temp == "ADD_ANS:")
            {
                std::getline(compInfoFile, temp);
                std::wstring ans = dom::Cyrilic::global.toWString(temp);
                dom::Cyrilic::global.standardProcedure(ans);
                questions[id].push_back(ans);
            }     
            else if (temp == "TYPE:")
            {
                std::getline(compInfoFile, temp);
                if(temp == " Standart")
                {
                    questionTypes[id] = 0;
                }
                else if(temp == " Multiple")
                {
                    questionTypes[id] = 1;
                    for(auto& k : questions[id])
                    {
                        dom::Cyrilic::global.destroyWhiteSpaces(k, true);
                    }
                }
            }  
            else if (temp == "WEIGHT:")
            {
                std::getline(compInfoFile, temp);
                questionWeight[id] = std::stod(temp);
            } 
            else
            {
                id = std::stoi(temp);
            }
        }
    }

    if (int(mFlags) & int(CommandFlag::PRINT_LOG)) 
    {
        std::wcout << L"\nAnswers:\n";
        for(auto& i : questions)
        {
            std::wcout << i.first << L" ";      
            for(int j = 0; j < i.second.size(); ++j)
            {
                if (j > 0) std::wcout << L" || ";
                std::wcout << i.second[j];
            }
            std::wcout << L"\n";
        }
        std::wcout << L"\n";

        std::wcout << L"Type:\n";
        for(auto& i : questionTypes)
        {
            std::wcout << i.first << L" " << int(i.second) << L"\n";
        }
        std::wcout << L"\n";

        std::wcout << L"Weight:\n";
        for(auto& i : questionWeight)
        {
            std::wcout << i.first << L" " << i.second << L"\n";
        }
        std::wcout << L"\n";
    }

    for(auto& uID : userIDs)
    {
        auto userAns = mDBQ.getUserAnswers(questionsIDs, uID);
        std::wcout << userNames[uID] << L" ";
        double s = 0;
        for(auto q : questions)
        {  
            if (int(mFlags) & int(CommandFlag::PRINT_LOG)) 
            {
                std::wcout << L"\n" << q.second[0] << L" === " <<  userAns[q.first]; 
            }
            int correctCount = 0;
            if (questionTypes[q.first] == 0)
            {
                for(auto& ans : q.second)
                {
                    if (ans == userAns[q.first]) 
                    {
                        correctCount = 1;
                        break;
                    }
                }
            }
            else if (questionTypes[q.first] == 1)
            {
                dom::Cyrilic::global.destroyWhiteSpaces(userAns[q.first], true);
                for(auto& ans : q.second)
                {
                    int cnt = 0;
                    for(int k = 0; k < ans.size(); ++k)
                    {
                        if (userAns[q.first].size() <= k) break;
                        if (ans[k] == userAns[q.first][k] && ans[k] != L' ') 
                        {
                            ++cnt;
                        }
                    }
                    correctCount = std::max(correctCount, cnt);
                }
            }


            if (int(mFlags) & int(CommandFlag::PRINT_LOG)) 
            {
                if (correctCount) 
                {
                    std::wcout << L" +";
                }
                else
                {
                    std::wcout << L" -";
                }
            }
            else
            {
                std::wcout << L"\t" << questionWeight[q.first] * correctCount;
            }

            if (correctCount) s+= questionWeight[q.first] * correctCount;
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