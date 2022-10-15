#include "file_manager.hpp"

//--------------------------------------------------------------------------------

#include "error_message.hpp"

//--------------------------------------------------------------------------------

void
dom::copyFile
(
    const std::string& aFromFileName,
    const std::string& aToFileName
) noexcept
{
    START_LOG_BLOCK("Copying_file");
    WRITE_LOG("From : ", aFromFileName);
    WRITE_LOG("To   : ", aToFileName);

    std::ifstream fromFile(aFromFileName);
    std::ofstream toFile(aToFileName);

    if (!fromFile.is_open()) 
    {
        WRITE_ERROR("File", "copyFile","01", 
            "CANT_OPEN_FILE_FOR_READING", aFromFileName);
    }
    if (!toFile.is_open()) 
    {
        WRITE_ERROR("File", "copyFile", "02", 
            "CANT_OPEN_FILE_FOR_WRITING", aToFileName);
    }

    std::string s;
    std::getline(fromFile, s, '\0');
    toFile << s;

    END_LOG_BLOCK();
    WRITE_LOG_ENDL();
}

//--------------------------------------------------------------------------------

dom::File::File(const std::string& aFileName) noexcept :
    mOut    (aFileName)
{}

//--------------------------------------------------------------------------------

dom::File::~File()
{
    mOut.close();
}

//--------------------------------------------------------------------------------

void
dom::File::setDelimiter(const std::string& aDelimiter) noexcept
{
    mDelimiter = aDelimiter;
}

//--------------------------------------------------------------------------------

void
dom::File::writeEndl() noexcept
{
    mOut << '\n';
}

//--------------------------------------------------------------------------------

void
dom::File::close() noexcept
{
    mOut.close();
}

//--------------------------------------------------------------------------------
