#ifndef USER_HPP
#define USER_HPP

//--------------------------------------------------------------------------------

#include <map>
#include <string>
#include <unordered_map>
#include <vector>

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

    struct Mask
    {
        enum class Function
        {
            AND = 1,
            OR  = 2,
            NOT = 3
        };
        std::wstring value;
        int funk;
    };

    UserArray() noexcept;
    ~UserArray() = default;

    UserArray(const UserArray& other) noexcept            = default;
    UserArray& operator=(const UserArray& other) noexcept = default;

    UserArray(UserArray&& other) noexcept            = default;
    UserArray& operator=(UserArray&& other) noexcept = default;

    std::map<IDType, User>::iterator begin() noexcept;
    std::map<IDType, User>::iterator end() noexcept;

    const std::map<IDType, User>::const_iterator begin() const noexcept;
    const std::map<IDType, User>::const_iterator end() const noexcept;

    void setName(IDType aID, std::wstring&& aName) noexcept;
    void setPassword(IDType aID, std::wstring&& aPassword) noexcept;
    void addAnswer(IDType aID, IDType aQaestionID,
                   std::wstring&& aAnswer) noexcept;

    const std::map<IDType, std::wstring>& getAnswers(IDType aID) const noexcept;
    // const std::map<int, std::wstring>& getAnswers(std::wstring aName);

    data::UserArray getUsers(const std::vector<Mask>& aMask = {},
                             bool aSwitchMask               = false) noexcept;
    data::UserArray getUsers(const std::vector<int>& aUserIDs) noexcept;

    void getPasswords(data::UserArray& aUsers) noexcept;

    void data::DatabaseQuery::getUserAnswers(
        const std::vector<int>& aUserIDs,
        const std::vector<int>& aQuestionNumbers) noexcept;

    void rename(const UserArray& aUsers,
                const std::vector<std::wstring>& aNewNames,
                bool aOnlyTest = false) noexcept;

private:
    Database& mDatabase;

    std::map<IDType, User> mUsers;
    // std::unordered_map<std::wstring, int> mNameHashs;
};

} // namespace data

//--------------------------------------------------------------------------------

#endif // !USER_HPP
