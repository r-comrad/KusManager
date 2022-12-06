#ifndef CORE_HPP
#define CORE_HPP

//--------------------------------------------------------------------------------

#include "domain/string.hpp"

#include "database/database_query.hpp"

#include "command.hpp"

//--------------------------------------------------------------------------------

namespace core
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

    void run(const std::vector<std::string>& argv) noexcept;
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

    Command mCommand;

    data::DatabaseQuery mDBQ;

    void printUsers() noexcept;
    void renameUsers() noexcept;

    int getUserCount() const noexcept;
    std::string getPrefix(std::string aDelimiter = "") noexcept;
    void generateNames() noexcept;
    void generateRoboNames() noexcept;

    void turn() noexcept;
};
} // namespace core

//--------------------------------------------------------------------------------

#endif // !CORE_HPP