#ifndef USER_HPP
#define USER_HPP

//--------------------------------------------------------------------------------

#include <map>
#include <string>
#include <unordered_map>

#include "user.hpp"

//--------------------------------------------------------------------------------

namespace data
{
struct User
{
public:
    std::wstring name;
    std::map<IDType, std::wstring> answers;
    std::wstring password;
};

class UserArray
{
public:
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

private:
    std::map<IDType, User> mUsers;
    // std::unordered_map<std::wstring, int> mNameHashs;
};

} // namespace data

//--------------------------------------------------------------------------------

#endif // !USER_HPP
