#include "file_reader.hpp"

dom::FileReader::FileReader(const std::string& aFileName) noexcept :
    mInp(aFileName)
{}

dom::FileReader::~FileReader()
{
    this->close();
}

std::vector<std::string>
dom::FileReader::getAllStrings() noexcept
{
    std::vector<std::string> result;
    std::string temp;
    while(getline(mInp, temp)) result.emplace_back(std::move(temp));
    return result;
}

void
dom::FileReader::close() noexcept
{
    mInp.close();
}

