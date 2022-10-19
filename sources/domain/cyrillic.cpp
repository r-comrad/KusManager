#include "cyrillic.hpp"

//--------------------------------------------------------------------------------

void 
Cyrilic::toLower(std::wstring& aStr) noexcept
{
    for(auto& i : aStr)
    {
        if (i == L'ё')
        {
            i = L'Ё';
        }
        else
        {
            i += L'а' - L'А';
        }
    }
}

//--------------------------------------------------------------------------------

void 
Cyrilic::toUpper(std::wstring& aStr) noexcept
{
    for(auto& i : aStr)
    {
        if (i == L'Ё')
        {
            i = L'ё';
        }
        else
        {
            i += L'А' - L'а';
        }
    }
}

//--------------------------------------------------------------------------------

std::wstring 
Cyrilic::toWString(const std::string& aStr) noexcept
{
    return toWString(aStr.c_str());
}

//--------------------------------------------------------------------------------

std::wstring 
Cyrilic::toWString(const char* aStr) noexcept
{
    std::wstring result;
    // auto str = const_cast<char*>(aStr);
    // u_char* s = (u_char*)str;
    
    for(u_char* c(s); *c !='\000'; ++c)
    {
        if (*c == 208 || *c == 209)
        {
            ss.emplace_back((*c==208 ? 896 : 960) + *++c);
        }
        else
        {
            ss.emplace_back(static_cast<wchar_t>(*c));
        }
    }
    return result;
}

//--------------------------------------------------------------------------------

void 
Cyrilic::destroyBadCharacters(std::wstring& aStr) noexcept
{
    for(auto& i : aStr)
    {
        if (i == L'ё')
        {
            i = L'е';
        }
        else if (i == L'Ё')
        {
            i = L'Е';
        }
        else if (i == L'й')
        {
            i = L'и';
        }
        else if (i == L'Й')
        {
            i = L'И';
        }
    }
}

//--------------------------------------------------------------------------------

char* 
Cyrilic::toLowerCyrillic(const char* aCharacter)
{
    char* res(new char[3]);
    res[2] = '\000';
    
    if (aCharacter[1] == '\201')
    {
        res[0] = '\321';
        res[1] = '\221';
    }
    else if (aCharacter[1] < '\240')
    {
        res[0] = '\320';
        res[1] = static_cast<char>(aCharacter[1] + ' ');
    }
    else
    {
        res[0] = '\321';
        res[1] = static_cast<char>(aCharacter[1] - ' ');
    }
    
    return res;
}

//--------------------------------------------------------------------------------

char* 
Cyrilic::toUpperCyrillic(const char* aCharacter)
{
    char* res(new char[3]);
    res[0] = '\320';
    res[2] = '\000';
    
    if (aCharacter[0] == '\320')
    {
        res[1] = static_cast<char>(aCharacter[1] - ' ');
    }
    else
    {
        if (aCharacter[1] == '\221')
        {
            res[1] = '\201';
        }
        else
        {
            res[1] = static_cast<char>(aCharacter[1] + ' ');
        }
    }
    
    return res;
}

//--------------------------------------------------------------------------------
