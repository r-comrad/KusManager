#include "code_info.hpp"

//--------------------------------------------------------------------------------

#include "main/path.hpp"

//--------------------------------------------------------------------------------

test::CodeInfo::CodeInfo(CodeInfoType aType) noexcept :
    mCodeData(3)
{
    mCodeData[size_t(CellMap::Submission)] = MAEDIA;
    if (aType == CodeInfoType::Submission)
    {
       mCodeData[size_t(CellMap::Output)] = SOLUTION_PATH + "sol-";
    }
    else
    {
       mCodeData[size_t(CellMap::Output)] = CHECKER_PATH + "ch-";
    }
}

//--------------------------------------------------------------------------------

test::CodeInfo::operator test::CPPInfo() noexcept
{
    return CPPInfo 
    { 
        std::move(mCodeData[size_t(CellMap::Submission)]), 
        std::move(mCodeData[size_t(CellMap::Output)]),
        std::move(mCodeData[size_t(CellMap::Language)])
    };
}

//--------------------------------------------------------------------------------

test::CodeInfo::operator test::PythonInfo() noexcept
{
    return PythonInfo
    { 
        std::move(mCodeData[size_t(CellMap::Submission)]), 
        std::move(mCodeData[size_t(CellMap::Language)])
    };
}

//--------------------------------------------------------------------------------

std::string
test::CodeInfo::getFileLanguage() const noexcept
{
    return std::string(mCodeData[size_t(CellMap::Language)]);
}

//--------------------------------------------------------------------------------

void
test::CodeInfo::setFileName(const dom::CharArray& aStr) noexcept
{
    mCodeData[size_t(CellMap::Submission)] += aStr;
}

//--------------------------------------------------------------------------------

void
test::CodeInfo::setDesirableOutputFileName(const dom::CharArray& aStr) noexcept
{
    mCodeData[size_t(CellMap::Output)] += aStr;
}

//--------------------------------------------------------------------------------

void
test::CodeInfo::setLanguage() noexcept
{
    auto ext = getFileExtension();
    START_LOG_BLOCK ("GET_FILE_EXTENSION");
    WRITE_LOG       ("HAS_FILE_EXTENSION:", ext.has_value());
    END_LOG_BLOCK   ("FILE_EXTENSION:",     ext.value());
    mCodeData[size_t(CellMap::Language)] = ext.value();
}

//--------------------------------------------------------------------------------

std::optional <std::string>
test::CodeInfo::getFileExtension() const noexcept
{
   return mCodeData[size_t(CellMap::Submission)].backSubStr('.');
}

//--------------------------------------------------------------------------------
