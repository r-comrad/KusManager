#include "command.hpp"

#include <fstream>

#include "domain/file_reader.hpp"

void
core::Command::process(const std::vector<std::string>& argv) noexcept
{
    processCommand(argv);
    processFlags(argv);
}

void
core::Command::processCommand(const std::vector<std::string>& argv) noexcept
{
    mCommand = Type::NUN;

    std::map<std::string, Type> commandMap{
        {"getUsers",          Type::GET_USERS          },
        {"getActiveUsers",    Type::GET_ACTIVE_USERS   },
        {"getDeletedUsers",   Type::GET_DELETED_USERS  },
        {"turnOffUsers",      Type::DELETE_USERS       },
        {"turnOnUsers",       Type::RESURRECT_USERS    },
        {"rename",            Type::RENAME_USERS       },
        {"generateNames",     Type::GENERATE_NAMES     },
        {"generateRoboNames", Type::GENERATE_ROBO_NAMES},
        {"getResults",        Type::EVALUATE           },
        {"help",              Type::HELP               }
    };

    for (auto& i : commandMap)
    {
        if (argv[1] == i.first)
        {
            mCommand = i.second;
            break;
        }
    }
}

void
core::Command::processFlags(const std::vector<std::string>& argv) noexcept
{
    mFlags = uint32_t(Flag::NUN);

    std::map<std::string, Flag> flagsMap{
        {"-test",   Flag::TEST       },
        {"-print",  Flag::PRINT      },
        {"-prefix", Flag::PREFIX     },
        {"-mask",   Flag::MASK       },
        {"-invert", Flag::INVERT_MASK},
        {"-all",    Flag::ALL_USERS  }
    };

    std::map<std::string, Flag> shortcutFlagsMap;
    for (auto& i : flagsMap)
    {
        std::string s{i.first[0], i.first[1]};
        for (auto j : i.first)
            if (std::isupper(j)) s.push_back(std::tolower(j));

        if (shortcutFlagsMap.count(s) == 0 && flagsMap.count(s) == 0)
        {
            shortcutFlagsMap[s] = i.second;
        }
        else
        {
            shortcutFlagsMap[s] = Flag::NUN;
        }
    }

    for (auto& i : shortcutFlagsMap)
    {
        if (i.second != Flag::NUN)
        {
            flagsMap[i.first] = i.second;
        }
    }

    size_t last = 0;
    for (int i = 2; i < argv.size(); ++i)
    {
        bool flag = false;
        for (auto& j : flagsMap)
        {
            if (argv[i] == j.first)
            {
                mFlags |= NumericFlagType(j.second);
                flag = true;
                last = int(j.second);
                break;
            }
        }

        if (flag)
        {
            mArgs[last].push_back(std::string(argv[i]));
        }
    }

    // if (int(mFlags) & int(Flag::HAS_MASK_FILE))
    // {
    //     mFlags |= uint32_t(Flag::HAS_MASK);
    //     auto mask = dom::FileReader::getAllStrings(
    //         mArgs[int(Flag::HAS_MASK_FILE)].front());

    //     for (auto& i : mask)
    //         mArgs[int(Flag::HAS_MASK)].emplace_back(std::move(i));
    // }
}

const core::Command::Type&
core::Command::getCommand() const noexcept
{
    return mCommand;
}

// const uint32_t&
// core::Command::getFlags() noexcept
// {
//     return mFlags;
// }

bool
core::Command::isCommand(Command::Type aType) const noexcept
{
    return mCommand == aType;
}

bool
core::Command::ifFlagSet(Command::Flag aFlag) const noexcept
{
    return mFlags & NumericFlagType(aFlag);
}

// const std::string&
std::optional<std::string>
core::Command::getArgCell(int aNum, size_t aCellNum) const noexcept
{
    std::optional<std::string> result;
    auto it = mArgs.find(aNum);
    if (it != mArgs.end())
    {
        if (it->second.size() > aCellNum)
        {
            result = it->second[aCellNum];
        }
    }
    return result;
}

// std::optional<const std::vector<std::string>&>
std::optional<std::vector<std::string>>
core::Command::getArgVector(Command::Flag aFlag) noexcept
{
    //    std::optional<const std::vector<std::string>&> result;
    std::optional<std::vector<std::string>> result;
    auto it = mArgs.find(NumericFlagType(aFlag));
    if (it != mArgs.end())
    {
        result = it->second;
    }
    return result;
}

const std::map<int, std::vector<std::string>>&
core::Command::getAllArgs() noexcept
{
    return mArgs;
}
