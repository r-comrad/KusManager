#ifndef COMMAND_HPP
#define COMMAND_HPP

//--------------------------------------------------------------------------------

#include <map>
#include <optional>
#include <string>
#include <vector>

//--------------------------------------------------------------------------------

namespace core
{
class Command
{
private:
    using NumericFlagType = uint32_t;

public:
    enum class Type
    {
        NUN,
        GET_USERS,
        GET_ACTIVE_USERS,
        GET_DELETED_USERS,
        DELETE_USERS,
        RESURRECT_USERS,
        RENAME_USERS,
        GENERATE_NAMES,
        GENERATE_ROBO_NAMES,
        EVALUATE,
        HELP
    };

    enum class Flag
    {
        NUN         = 0,
        TEST        = 1,
        PRINT       = 2,
        PREFIX      = 4,
        MASK        = 8,
        INVERT_MASK = 16
    };

    Command() noexcept = default;
    ~Command()         = default;

    Command(const Command& other) noexcept            = delete;
    Command& operator=(const Command& other) noexcept = delete;

    Command(Command&& other) noexcept            = default;
    Command& operator=(Command&& other) noexcept = default;

    void process(const std::vector<std::string>& argv) noexcept;

    const Type& getCommand() const noexcept;
    // const uint32_t& getFlags() noexcept;
    bool isCommand(Command::Type aType) const noexcept;
    bool hasFlag(Command::Flag aFlag) const noexcept;

    std::optional<std::string> getArgCell(int aNum,
                                          size_t aCellNum) const noexcept;

    // TODO: referance
    // std::optional<const std::vector<std::string>&> getArgVector(
    std::optional<std::vector<std::string>> getArgVector(
        Command::Flag aFlag) noexcept;
    const std::map<int, std::vector<std::string>>& getAllArgs() noexcept;

private:
    Type mCommand;
    NumericFlagType mFlags;

    std::map<int, std::vector<std::string>> mArgs;

    void processCommand(const std::vector<std::string>& argv) noexcept;
    void processFlags(const std::vector<std::string>& argv) noexcept;
};
} // namespace core

//--------------------------------------------------------------------------------

#endif // !COMMAND_HPP
