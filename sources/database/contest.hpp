#ifndef CONTEST_HPP
#define CONTEST_HPP

//--------------------------------------------------------------------------------

#include <map>
#include <string>
#include <unordered_map>

#include "database.hpp"

//--------------------------------------------------------------------------------

using IDType = uint64_t;
namespace data
{
class Contest
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

    IDType mCompetitionId;
    std::wstring mCompetitionName;
    std::map<IDType, std::wstring> mGroups;
    std::map<IDType, Quaestion> mQuaestions;
    std::map<std::wstring, IDType> mQuaestionNames;

    Contest(const std::wstring& aCompetitionName,
            const std::wstring& aGroupName) noexcept;
    ~Contest() = default;

    Contest(const Contest& other) noexcept            = default;
    Contest& operator=(const Contest& other) noexcept = default;

    Contest(Contest&& other) noexcept            = default;
    Contest& operator=(Contest&& other) noexcept = default;

    std::vector<IDType> getUserIDs() const noexcept;

private:
    Database& mDatabase;

    void setCompetitionInfo(const std::wstring& aCompetitionName,
                            const std::wstring& aGroupName) noexcept;
    void setQuaestionID() noexcept;
    void setQuaestionName() noexcept;
    void setAnswers() noexcept;
};

} // namespace data

//--------------------------------------------------------------------------------

#endif // !CONTEST_HPP
