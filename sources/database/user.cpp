#include "user.hpp"

data::UserArray::UserArray() noexcept : mDatabase(Database::getDatabase())
{
}

std::map<IDType, data::User>::iterator
data::UserArray::begin() noexcept
{
    return mUsers.begin();
}

std::map<IDType, data::User>::iterator
data::UserArray::end() noexcept
{
    return mUsers.end();
}

const std::map<IDType, data::User>::const_iterator
data::UserArray::begin() const noexcept
{
    return mUsers.cbegin();
}

const std::map<IDType, data::User>::const_iterator
data::UserArray::end() const noexcept
{
    return mUsers.cend();
}

void
data::UserArray::setName(IDType aID, std::wstring&& aName) noexcept
{
    mUsers[aID].name = std::move(aName);
}

void
data::UserArray::setPassword(IDType aID, std::wstring&& aPassword) noexcept
{
    mUsers[aID].password = std::move(aPassword);
}

void
data::UserArray::addAnswer(IDType aID, IDType aQaestionID,
                           std::wstring&& aAnswer) noexcept
{
    mUsers[aID].answers[aQaestionID] = std::move(aAnswer);
}

const std::map<IDType, std::wstring>&
data::UserArray::getAnswers(IDType aID) const noexcept
{
    return mUsers.find(aID)->second.answers;
}

data::UserArray
data::DatabaseQuery::getUsers(const std::vector<UserArray::Mask>& aMask,
                              bool aSwitchMask) noexcept
{
    data::UserArray result;

    mDatabase.select(L"core_users", L"id, username, is_superuser, "
                                    L"is_teacher, is_staff");
    while (true)
    {
        mDatabase.step();
        if (!mDatabase.hasData()) break;

        if (mDatabase.getBool(2)) continue;
        if (mDatabase.getBool(3)) continue;
        if (mDatabase.getBool(4)) continue;

        auto id   = mDatabase.getIntFromRow(0).value();
        auto name = mDatabase.getText16FromRow(1).value();

        if (isUserHasBron(name)) continue;

        bool flagOr  = false;
        bool flagAnd = true;
        for (auto& i : aMask)
        {
            bool temp = name.find(i.value);
            if (i.funk & int(Mask::Function::NOT)) temp ^= true;
            if (i.funk & int(Mask::Function::AND)) flagAnd &= temp;
            else flagOr |= temp;
        }

        if ((flagOr && flagAnd) ^ (!aSwitchMask)) continue;

        result.setName(id, std::move(name));
    }
    mDatabase.closeStatment();

    return result;
}

data::UserArray
data::DatabaseQuery::getUsers(const std::vector<int>& aUserIDs) noexcept
{
    data::UserArray result;

    for (auto user : aUserIDs)
    {
        mDatabase.select(L"core_users",
                         L"username, is_superuser, is_teacher, is_staff",
                         L"id = " + std::to_wstring(user));

        if (mDatabase.getBool(2)) continue;
        if (mDatabase.getBool(3)) continue;
        if (mDatabase.getBool(4)) continue;

        mDatabase.step();
        auto name = mDatabase.getText16FromRow(0);
        mDatabase.closeStatment();
        if (!name.has_value()) break;

        if (isUserHasBron(name.value())) continue;
        result.setName(user, std::move(name.value()));
    }

    return result;
}
//--------------------------------------------------------------------------------

void
data::DatabaseQuery::getPasswords(data::UserArray& aUsers) noexcept
{
    for (auto& i : aUsers)
    {
        mDatabase.select(L"core_passwords", L"password",
                         L"user_id = " + std::to_wstring(i.first));

        mDatabase.step();
        i.second.password = mDatabase.getText16FromRow(0).value();
        mDatabase.closeStatment();
    }
}

void
data::DatabaseQuery::rename(const UserArray& aUsers,
                            const std::vector<std::wstring>& aNewNames,
                            bool aOnlyTest) noexcept
{
    auto itUsers    = aUsers.begin();
    auto itNewNames = aNewNames.begin();
    while (itUsers != aUsers.end() && itNewNames != aNewNames.end())
    {
        if (aNewNames.size() == 0) break;
        wprintf(L"%s   ----->   %s\n", itUsers->second.name.c_str(),
                itNewNames->c_str());

        if (!aOnlyTest)
        {
            mDatabase.update(L"core_users",
                             L"username = \'" + *itNewNames + L"\'",
                             L"id = " + std::to_wstring(itUsers->first));
            mDatabase.step();
            mDatabase.closeStatment();
        }

        itUsers++;
        itNewNames++;
    }
}
