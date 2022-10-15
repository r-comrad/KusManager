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

// #define _CRT_SECURE_NO_WARNINGS

// #include <set>
// #include <iostream>
// #include <string>
// #include <algorithm>
// #include <vector>
// #include <sstream>
// using namespace std;

// int partition(vector<int>& arr, int l, int r)
// {
//     //int sum = arr[(l + r) / 2];
//     long long sum = 0;
//     for (int i = l; i <= r; ++i)
//     {
//         sum += arr[i];
//     }
//     sum /= r - l + 1;

//     int i = l;
//     int j = r;
//     while (i <= j)
//     {
//         while (i < arr.size() && arr[i] < sum)
//         {
//             i++;
//         }
//         while (j >= 0 && arr[j] > sum)
//         {
//             j--;
//         }
//         if (i >= j || j < 0 || i >= arr.size())
//         {
//             break;
//         }
//         swap(arr[i++], arr[j--]);
//     }
//     if (l == j)
//     {
//         int mn = l;
//         for (int i = l; i < r; ++i) if (arr[mn] < arr[i]) mn = i;
//         j++;
//         swap(arr[mn], arr[j + 1]);
//     }
//     return j;
// }

// int findOrderStatistic(vector<int>& arr, int k)
// {
//     int left = 0, right = arr.size() - 1;
//     int res;
//     while (true)
//     {
//         int mid = partition(arr, left, right);

//         if (right - left == 1)
//         {
//             return arr[k];
//         }
//         else if (k <= mid) 
//         {
//             right = mid;
//         }
//         else 
//         {
//             left = mid;
//         }
//     }
// }

// int main()
// {
//     freopen("a.in", "r", stdin);
//     freopen("a.out", "w", stdout);

//     vector<string> ans;
//     string s;
//     getline(cin, s);
//     std::stringstream ss;
//     ss << s;
//     while (ss >> s)
//     {
//         for(auto& i : s) i = std::tolower(i);
//         ans.emplace_back(std::move(s));
//     }

//     while (getline(cin, s))
//     {
//         std::stringstream ss;
//         ss << s;
//         ss >> s;
//         cout << s << "\t";

//         int cnt = 0;
//         for(int num = 0; ss >> s; ++num)
//         {
//             if (s == "NO_ANS" || s == "NO_ANS;") continue;
//             for (auto& i : s) i = std::tolower(i);
//             for (size_t i = 0; i < s.size() &&
//                 num < ans.size() && i < ans[num].size(); ++i)
//             {
//                 if (s[i] == ans[num][i]) ++cnt;
//             }
//         }
//         cout << cnt << "" << endl;
//     }

//   return 0;
// }