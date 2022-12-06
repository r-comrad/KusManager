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
        NUN                 = 0,
        GET_USERS           = 1,
        GET_ACTIVE_USERS    = 2,
        GET_DELETED_USERS   = 3,
        DELETE_USERS        = 4,
        RESURRECT_USERS     = 5,
        RENAME_USERS        = 6,
        GENERATE_NAMES      = 7,
        GENERATE_ROBO_NAMES = 8,
        EVALUATE            = 9,
        HELP                = 10
    };

    enum class Flag
    {
        NUN         = 0,
        TEST        = 1,
        PRINT       = 2,
        PREFIX      = 4,
        MASK        = 8,
        INVERT_MASK = 16,
        ALL_USERS   = 32
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
    bool ifFlagSet(Command::Flag aFlag) const noexcept;

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
