#ifndef CYRILLIC_HPP
#define CYRILLIC_HPP

//--------------------------------------------------------------------------------

#include <fstream>
#include <string>
#include <wstring>

//--------------------------------------------------------------------------------

namespace dom
{
    class Cyrilic
    {
    public:
        static Cyrilic global;

        void toLower(std::wstring& aStr) noexcept;
        void toUpper(std::wstring& aStr) noexcept;

        std::wstring toWString(const std::string& aStr) noexcept;
        std::wstring toWString(const char* aStr) noexcept;

        void destroyBadCharacters(std::wstring& aStr) noexcept;
        void destroyBadCharacters(std::wstring& aStr) noexcept;

    private:
        Cyrilic() noexcept;
        ~Cyrilic();

        Cyrilic(const Cyrilic& other) = delete;
        Cyrilic& operator=(const Cyrilic& other) = delete;

        Cyrilic(Cyrilic&& other)  noexcept= default;
        Cyrilic& operator=(Cyrilic&& other) noexcept = default;

        char* toLowerCyrillic(const char* aCharacter) noexcept;
        char* toUpperCyrillic(const char* aCharacter) noexcept;
    };
}

//--------------------------------------------------------------------------------

#endif // !CYRILLIC_HPP
