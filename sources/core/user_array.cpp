#include "user_array.hpp"

#include "domain/file_reader.hpp"

void
core::UserArray::makeNames(int* n) noexcept
{
    std::ofstream output("out.name");

    std::ifstream file();
    std::vector<std::string> firstNames =
        dom::FileReader(sFirstNameFileName).getAllStrings();

    std::vector<std::string> secondName =
        dom::FileReader(sSecondNameFileName).getAllStrings();

    file.open("names.txt");
    while (file >> inp)
    {
        std::string add;
        file >> add;
        std::vector<int> size(7);
        for (auto& i : size)
        {
            file >> i;
            if (i)
            {
                if (i / 2 > 50)
                    i += 50;
                else
                    i += 25;
            }
        }

        if (inp == "-s")
        {
            for (int i = 0; i < size.size(); ++i)
            {
                std::set<std::string> names;
                while (names.size() < size[i])
                {
                    names.insert(firstNames[rand() % firstNames.size()] + "-" +
                                 secondName[rand() % secondName.size()]);
                }

                for (auto& j : names)
                {
                    std::string full_name =
                        add + "-" + std::to_string(i + 5) + "-" + j + "\n";
                    output << full_name;
                }
                output << "\n";
            }
        }
        else if (inp == "-n")
        {
            for (int i = 0; i < size.size(); ++i)
            {
                for (auto& i : add)
                    i = std::tolower(i);
                std::set<std::string> names;
                while (names.size() < size[i])
                {
                    std::string temp;
                    temp.reserve(12);
                    while (temp.size() < 12)
                    {
                        temp.push_back(rand() % ('z' - 'a') + 'a');
                    }
                    names.insert(temp);
                }
                for (auto& j : names)
                {
                    std::string full_name =
                        add + std::to_string(i + 5) + j + "\n";
                    output << full_name;
                }
                output << "\n";
            }
        }

        output << "\n";
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
    while (file >> inp)
        firstNames.push_back(std::move(inp));
    file.close();

    file.open("secondName.txt");
    std::vector<std::string> secondName;
    while (file >> inp)
        secondName.push_back(std::move(inp));
    file.close();

    std::set<std::string> names;
    while (names.size() < count)
    {
        names.insert(firstNames[rand() % firstNames.size()] +
                     secondName[rand() % secondName.size()]);
    }

    std::ofstream output(NEW_NAMES_FILENAME);
    bool maskFlag = int(mFlags) & int(CommandFlag::HAS_MASK);
    for (auto& i : names)
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
