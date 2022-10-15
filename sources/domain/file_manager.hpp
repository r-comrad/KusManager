#ifndef FILES_MANAGER_HPP
#define FILES_MANAGER_HPP

//--------------------------------------------------------------------------------

#include <fstream>
#include <string>

//--------------------------------------------------------------------------------

namespace dom
{
    void copyFile(const std::string& aFromFileName, 
        const std::string& aToFileName) noexcept;

    class File
    {
    public:
        File(const std::string& aFileName) noexcept;
        ~File();

        File(const File& other) = delete;
        File& operator=(const File& other) = delete;

        File(File&& other)  noexcept= default;
        File& operator=(File&& other) noexcept = default;

        template<typename... Args>
        void write(Args... args) noexcept
        {
            (void) std::initializer_list<bool>
            { 
                static_cast<bool>(mOut << args << mDelimiter)...
            };
            mOut << '\n';
        }

        void setDelimiter(const std::string& aDelimiter) noexcept;
        void writeEndl() noexcept;

        void close() noexcept;

    private:
        std::ofstream mOut;
        std::string mDelimiter;
    };
}

//--------------------------------------------------------------------------------

#endif // !FILES_MANAGER_HPP
