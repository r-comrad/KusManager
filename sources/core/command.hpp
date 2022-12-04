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
        NUN           = 0,
        TEST          = 1,
        PRINT         = 2,
        HAS_PREFIX    = 4,
        HAS_MASK_FILE = 8,
        INVERT_MASK   = 16,
        ALL_USERS     = 32
    };

    Command() noexcept = default;
    ~Command()         = default;

    Command(const Command& other) noexcept            = delete;
    Command& operator=(const Command& other) noexcept = delete;

    Command(Command&& other) noexcept            = default;
    Command& operator=(Command&& other) noexcept = default;

    void process(const std::vector<std::string>& argv) noexcept;

    const Type& getCommand() noexcept;
    const uint32_t& getFlags() noexcept;

    std::optional<std::string> getArgCell(int aNum, size_t aCellNum) noexcept;

    // TODO: referance
    // std::optional<const std::vector<std::string>&> getArgVector(
    std::optional<std::vector<std::string>> getArgVector(int aNum) noexcept;
    const std::map<int, std::vector<std::string>>& getAllArgs() noexcept;

private:
    Type mCommand;
    uint32_t mFlags;

    std::map<int, std::vector<std::string>> mArgs;

    void processCommand(const std::vector<std::string>& argv) noexcept;
    void processFlags(const std::vector<std::string>& argv) noexcept;
};
} // namespace core

//--------------------------------------------------------------------------------

#endif // !COMMAND_HPP
