#include "user.hpp"

#include "domain/cyrillic.hpp"

const std::wstring data::UserArray::DELETE_PREFIX = L"-----";

data::UserArray::UserArray(const std::vector<Mask>& aMask) noexcept
    : mDatabase(Database::getDatabase())
{
    setUsers(aMask);
}

data::UserArray::UserArray(const Contest& aContest) noexcept
    : mDatabase(Database::getDatabase())
{
    setUsers(aContest.getUserIDs());
}

std::map<IDType, data::UserArray::User>::iterator
data::UserArray::begin() noexcept
{
    return mUsers.begin();
}

std::map<IDType, data::UserArray::User>::iterator
data::UserArray::end() noexcept
{
    return mUsers.end();
}

const std::map<IDType, data::UserArray::User>::const_iterator
data::UserArray::begin() const noexcept
{
    return mUsers.cbegin();
}

const std::map<IDType, data::UserArray::User>::const_iterator
data::UserArray::end() const noexcept
{
    return mUsers.cend();
}

const std::map<IDType, std::wstring>&
data::UserArray::getAnswers(IDType aID) const noexcept
{
    return mUsers.find(aID)->second.answers;
}

//--------------------------------------------------------------------------------

void
data::UserArray::setPasswords() noexcept
{
    for (auto& i : mUsers)
    {
        mDatabase.select(L"core_passwords", L"password",
                         L"user_id = " + std::to_wstring(i.first));

        mDatabase.step();
        i.second.password = mDatabase.getText16FromRow(0).value();
        mDatabase.closeStatment();
    }
}

void
data::UserArray::setAnswers(const Contest& aContest) noexcept
{
    for (auto& user : mUsers)
    {
        auto userId   = user.first;
        auto& answers = user.second.answers;
        for (auto question : aContest.mQuaestions)
        {
            auto questionId = question.first;
            mDatabase.select(L"core_questionans", L"ans",
                             L"question_id = " + std::to_wstring(questionId) +
                                 L" AND user_id = " + std::to_wstring(userId));

            while (true)
            {

                mDatabase.step();
                auto ans = mDatabase.getText16FromRow(0);

                if (!ans.has_value())
                {
                    mDatabase.closeStatment();
                    break;
                }
                answers[questionId] = ans.value();
                dom::Cyrilic::global.standardProcedure(answers[questionId]);
            }
            mDatabase.closeStatment();
        }
    }
}

void
data::UserArray::rename(const std::vector<std::wstring>& aNewNames,
                        bool aOnlyTest) noexcept
{
    auto itUsers    = mUsers.begin();
    auto itNewNames = aNewNames.begin();
    while (itUsers != mUsers.end() && itNewNames != aNewNames.end())
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

void
data::UserArray::print() const noexcept
{
    for (auto& i : mUsers)
    {
        wprintf(L"%s\t\t%s\n", i.second.name.c_str(),
                i.second.password.c_str());
    }
}

void
data::UserArray::setUsers(
    const std::vector<data::UserArray::Mask>& aMask) noexcept
{
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

        if ((flagOr && flagAnd)) continue;

        mUsers[id].name = std::move(name);
    }
    mDatabase.closeStatment();
}

void
data::UserArray::setUsers(const std::vector<IDType>& aUserIDs) noexcept
{
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
        mUsers[user].name = std::move(name.value());
    }
}

bool
data::UserArray::isUserHasBron(const std::wstring& aName)
{
    return aName == L"pashs" || aName == L"ITMO-Student" ||
           aName == L"AlphaPrimus" || aName == L"Ivan" ||
           aName == L"Ivanus-Primus";
}
