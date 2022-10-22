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

        Core(const Core& other) noexcept = default;
        Core& operator=(const Core& other) noexcept = default;

        Core(Core&& other) noexcept = default;
        Core& operator=(Core&& other) noexcept = default;
        
        void run(int argc, char* argv[]) noexcept;
        void makeNames() noexcept;
        void getResults(const std::string& aCompetitionName) noexcept;
    private:
        enum class Command
        {
            NUN = 0, 
            GET_USERS = 1, 
            GET_ACTIVE_USERS = 2,
            GET_DELETED_USERS = 3,
            DELETE_USERS = 4,
            RESURRECT_USERS = 5,
            RENAME_USERS = 6,
            GENERATE_NAMES = 7,
            GENERATE_ROBO_NAMES = 8
        };

        enum class CommandFlag
        {
            NUN = 0,
            TEST = 1,
            PRINT_LOG = 2,
            HAS_MASK = 4,
            HAS_MASK_FILE = 8,
            INVERT_MASK = 16,
            ALL_USERS = 32
        };

        data::DatabaseQuery mDBQ;

        Command mCommand;
        CommandFlag mFlags;
        std::map<int, std::vector<std::string>> mArgs;

        void parseCommandLine(int argc, char* argv[]) noexcept;
        void printUsers() noexcept;
        void renameUsers() noexcept;
        void generateNames() noexcept;
        void generateRoboNames() noexcept;
        void turn() noexcept;

    };
}

//--------------------------------------------------------------------------------

#endif // !CORE_HPP