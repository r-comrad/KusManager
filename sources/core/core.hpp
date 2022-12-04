#ifndef CORE_HPP
#define CORE_HPP

//--------------------------------------------------------------------------------

#include "domain/string.hpp"

#include "database/database_query.hpp"

//--------------------------------------------------------------------------------

namespace eval
{
class Core
{
public:
    Core(const std::string& aDatabasePath) noexcept;
    ~Core() = default;

    Core(const Core& other) noexcept            = default;
    Core& operator=(const Core& other) noexcept = default;

    Core(Core&& other) noexcept            = default;
    Core& operator=(Core&& other) noexcept = default;

    void run(std::vector<std::string> argv) noexcept;
    void makeNames() noexcept;
    void getResults() noexcept;

private:
    enum class QuestionType
    {
        Standart,
        Multiple,
        MultipleStrong,
        Complex
    };
    struct Question
    {
        int id = 0;
        std::wstring name;
        std::vector<std::wstring> ans;
        QuestionType type = QuestionType::Standart;
        double weight     = 1;
    };

    enum class Command
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
        EVALUATE            = 9
    };

    enum class CommandFlag
    {
        NUN           = 0,
        TEST          = 1,
        PRINT         = 2,
        HAS_MASK      = 4,
        HAS_MASK_FILE = 8,
        INVERT_MASK   = 16,
        ALL_USERS     = 32
    };

    data::DatabaseQuery mDBQ;

    Command mCommand;
    CommandFlag mFlags;
    std::map<int, std::vector<std::string>> mArgs;

    void parseCommandLine(std::vector<std::string> argv) noexcept;
    void printUsers() noexcept;
    void renameUsers() noexcept;
    void generateNames() noexcept;
    void generateRoboNames() noexcept;
    void turn() noexcept;
};
} // namespace eval

//--------------------------------------------------------------------------------

#endif // !CORE_HPP