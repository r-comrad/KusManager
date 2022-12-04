#ifndef UESR_ARRAY_HPP
#define UESR_ARRAY_HPP

//--------------------------------------------------------------------------------

#include "domain/string.hpp"

#include "database/database_query.hpp"

//--------------------------------------------------------------------------------

namespace core
{
class UserArray
{
public:
    UserArray() noexcept;
    ~UserArray() = default;

    UserArray(const Core& other) noexcept            = default;
    UserArray& operator=(const Core& other) noexcept = default;

    UserArray(Core&& other) noexcept            = default;
    UserArray& operator=(Core&& other) noexcept = default;

    void makeNames() noexcept;

private:
    static const std::string sFirstNameFileName  = "firstName.txt";
    static const std::string sSecondNameFileName = "secondName.txt";

    void parseCommandLine(int argc, char* argv[]) noexcept;
    void printUsers() noexcept;
    void renameUsers() noexcept;
    void generateNames() noexcept;
    void generateRoboNames() noexcept;
    void turn() noexcept;
};
} // namespace core

//--------------------------------------------------------------------------------

#endif // !UESR_ARRAY_HPP