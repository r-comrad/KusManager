#include "contest.hpp"

#include "domain/cyrillic.hpp"

data::Contest::Contest(const std::wstring& aCompetitionName,
                       const std::wstring& aGroupName) noexcept
    : mDatabase(Database::getDatabase())
{
    setCompetitionInfo(aCompetitionName, aGroupName);
    setQuaestionID();
    setQuaestionName();
    setAnswers();
}

std::vector<IDType>
data::Contest::getUserIDs() const noexcept
{
    std::vector<IDType> result;

    for (auto groupNum : mGroups)
    {
        mDatabase.select(L"core_studentgroup_users", L"users_id",
                         L"studentgroup_id = \'" +
                             std::to_wstring(groupNum.first) + L"\'");

        while (true)
        {

            mDatabase.step();
            auto num = mDatabase.getInt64FromRow(0);

            if (!num.has_value())
            {
                mDatabase.closeStatment();
                break;
            }
            result.push_back(num.value());
        }
        mDatabase.closeStatment();
    }

    return result;
}

void
data::Contest::setCompetitionInfo(const std::wstring& aCompetitionName,
                                  const std::wstring& aGroupName) noexcept
{
    mDatabase.select(L"core_competitions", L"id, start_time, end_time",
                     L"name = \'" + aCompetitionName + L"\'");
    mDatabase.step();
    auto id = mDatabase.getInt64FromRow(0);
    mDatabase.closeStatment();

    if (!id.has_value())
    {
        wprintf(L"No such contest found!\n");
        exit(0);
    }

    mCompetitionId   = id.value();
    mCompetitionName = aCompetitionName;

    mDatabase.select(L"core_studentgroup_competitions", L"studentgroup_id",
                     L"competitions_id = " + L"\'"s +
                         std::to_wstring(mCompetitionId) + L"\'");
    while (true)
    {
        mDatabase.step();
        auto num = mDatabase.getInt64FromRow(0);

        if (!num.has_value()) break;
        mGroups[num.value()];
    }
    mDatabase.closeStatment();

    if (aGroupName != L"")
    {
        mDatabase.select(L"core_studentgroup", L"id",
                         L"name = " + L"\'"s + aGroupName + L"\'");
        auto group = mDatabase.getInt64FromRow(0);
        mDatabase.closeStatment();
        if (!group.has_value())
        {
            wprintf(L"No such group found!\n");
            exit(0);
        }

        auto groupId = group.value();
        bool flag    = false;
        for (auto& i : mGroups)
            flag |= i.first == groupId;
        if (flag == false)
        {
            wprintf(L"No connection between group and contest found!\n");
            exit(0);
        }

        mGroups.clear();
        mGroups[groupId] = aGroupName;
    }
}

void
data::Contest::setQuaestionID() noexcept
{
    mDatabase.select(L"core_competitions_questions", L"question_id",
                     L"competitions_id = " + L"\'"s +
                         std::to_wstring(mCompetitionId) + L"\'");
    while (true)
    {

        mDatabase.step();
        auto num = mDatabase.getInt64FromRow(0);
        if (!num.has_value()) break;
        mQuaestions[num.value()];
    }
    mDatabase.closeStatment();
}

void
data::Contest::setQuaestionName() noexcept
{
    for (auto& quaestion : mQuaestions)
    {
        auto id = quaestion.first;
        mDatabase.select(L"core_question", L"name",
                         L"id = " + std::to_wstring(id));
        mDatabase.step();
        mQuaestionNames[mDatabase.getText16FromRow(0).value()] = id;
        mDatabase.closeStatment();
    }
}

void
data::Contest::setAnswers() noexcept
{
    for (auto questionIt : mQuaestions)
    {
        auto questionID = questionIt.first;
        auto& question  = questionIt.second;

        mDatabase.select(L"core_question", L"question",
                         L"id = \'" + std::to_wstring(questionID) + L"\'");

        while (true)
        {
            mDatabase.step();
            auto ans = mDatabase.getText16FromRow(0);

            if (!ans.has_value())
            {
                mDatabase.closeStatment();
                break;
            }
            std::wstring s = ans.value();

            s.resize(s.size() - 4);
            std::wstring ss;
            std::reverse(s.begin(), s.end());
            s.resize(s.size() - 23);
            if (s.back() == L'\"')
            {
                for (auto& i : s)
                {
                    if (i == L' ') i = L'_';
                }
                for (auto& i : s)
                {
                    if (i == L'\"' || i == L'\\' || i == L'[' || i == L']' ||
                        i == L':' || i == L',')
                        i = L' ';
                }
                std::reverse(s.begin(), s.end());

                std::wstringstream sss;
                sss << s;
                for (int i = 0; sss >> s; ++i)
                {
                    sss >> s;
                    sss >> s;
                    if (s == L"true") ss += std::to_wstring(i) + L" ";
                }

                // ss = s;
            }
            else
            {
                std::reverse(s.begin(), s.end());
                ss = s;
            }
            question.answer = ss;
            dom::Cyrilic::global.standardProcedure(question.answer);
        }
        mDatabase.closeStatment();
    }
}
