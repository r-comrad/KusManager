#ifndef CONTEST_HPP
#define CONTEST_HPP

//--------------------------------------------------------------------------------

#include <map>
#include <string>
#include <unordered_map>

//--------------------------------------------------------------------------------

using IDType = uint64_t;

struct Contest
{
public:
    struct Quaestion
    {
        enum Type
        {
            Standart = 1
        };
        double value;
        Type type;
        std::wstring answer;
    };

    void setName(IDType aID, std::wstring&& aName) noexcept;
    void setPassword(IDType aID, std::wstring&& aPassword) noexcept;
    void addAnswer(IDType aID, IDType aQaestionID,
                   std::wstring&& aAnswer) noexcept;

    const std::map<IDType, std::wstring>& getAnswers(int aID) const noexcept;

    IDType mCompetitionId;
    std::wstring mCompetitionName;
    std::map<IDType, std::wstring> mGroups;
    std::map<IDType, Quaestion> mQuaestions;
};

// namespace data

//--------------------------------------------------------------------------------

#endif // !CONTEST_HPP
