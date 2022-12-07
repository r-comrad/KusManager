#ifndef USER_HPP
#define USER_HPP

//--------------------------------------------------------------------------------

#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include "domain/string.hpp"

#include "database.hpp"

#include "contest.hpp"
//--------------------------------------------------------------------------------

namespace data
{

class UserArray
{
public:
    struct User
    {
    public:
        std::wstring name;
        std::map<IDType, std::wstring> answers;
        std::wstring password;
    };

    static const std::wstring DELETE_PREFIX;
    struct Mask
    {
        enum class Function
        {
            AND = 1,
            OR  = 2,
            NOT = 4
        };
        std::wstring value;
        int funk;
    };

    UserArray(const std::vector<Mask>& aMask = {}) noexcept;
    UserArray(const Contest& aContest) noexcept;
    ~UserArray() = default;

    UserArray(const UserArray& other) noexcept            = default;
    UserArray& operator=(const UserArray& other) noexcept = default;

    UserArray(UserArray&& other) noexcept            = default;
    UserArray& operator=(UserArray&& other) noexcept = default;

    std::map<IDType, User>::iterator begin() noexcept;
    std::map<IDType, User>::iterator end() noexcept;

    const std::map<IDType, User>::const_iterator begin() const noexcept;
    const std::map<IDType, User>::const_iterator end() const noexcept;

    const std::map<IDType, std::wstring>& getAnswers(IDType aID) const noexcept;

    void setPasswords() noexcept;

    void setAnswers(const Contest& aContest) noexcept;

    void rename(const std::vector<std::wstring>& aNewNames,
                bool aOnlyTest = false) noexcept;

    void print() const noexcept;

private:
    Database& mDatabase;

    std::map<IDType, User> mUsers;

    void setUsers(const std::vector<Mask>& aMask) noexcept;
    void setUsers(const std::vector<IDType>& aUserIDs) noexcept;

    bool isUserHasBron(const std::wstring& aName);
};

} // namespace data

//--------------------------------------------------------------------------------

#endif // !USER_HPP
