#include "user.hpp"

std::map<int, data::User>::iterator
data::UserArray::begin() noexcept
{
    return mUsers.begin();
}

std::map<int, data::User>::iterator
data::UserArray::end() noexcept
{
    return mUsers.end();
}

const std::map<int, data::User>::const_iterator
data::UserArray::begin() const noexcept
{
    return mUsers.cbegin();
}

const std::map<int, data::User>::const_iterator
data::UserArray::end() const noexcept
{
    return mUsers.cend();
}

void
data::UserArray::setName(int aID, std::wstring&& aName) noexcept
{
    mUsers[aID].name = std::move(aName);
}

void
data::UserArray::setPassword(int aID, std::wstring&& aPassword) noexcept
{
    mUsers[aID].password = std::move(aPassword);
}

void
data::UserArray::addAnswer(int aID, int aQaestionID,
                           std::wstring&& aAnswer) noexcept
{
    mUsers[aID].answers[aQaestionID] = std::move(aAnswer);
}

const std::map<int, std::wstring>&
data::UserArray::getAnswers(int aID) const noexcept
{
    return mUsers.find(aID)->second.answers;
}
