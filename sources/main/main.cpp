//#define 

#include <string>

#include "domain/error_message.hpp"

#include "path.hpp"

#include "core/core.hpp"

//--------------------------------------------------------------------------------
#include <fstream>
int main(int argc, char* argv[])
{
//     wchar_t c = L'Ф';
//     std::wcout << c << L'\n';
//     std::wcout << L'ф' << L'\n';
//     std::wcout << wchar_t(c - L'А' + L'а') << L'\n';
//     std::wcout << (L'ф' == wchar_t(c - L'А' + L'а'))<< L'\n';
// return 0;
// int cnt = 0;
// for(wchar_t i = L'А'; i < L'Я'; ++i)
// {
//     std::wcout << ++cnt << L": " << i  << L" - " << int(i) << L"\n";
// }
// cnt = 0;
// for(wchar_t i = L'а'; i < L'я'; ++i)
// {
//     std::wcout << ++cnt << L": " << i << L" - " << int(i) << L"\n";
// }

//     std::wofstream out;
//     out.open("aaa.txt");
//     out << L"аямвав";
// out.close();
//  return 0;

//setlocale( LC_ALL,"Russian" ); 
//freopen("aaa", "w", stdout);
    WRITE_LOG("Start_main");
    eval::Core core("");

    if (argc < 2)
    {
        freopen("a.out", "w", stdout);
        //int sz = 5;
        //char* gg[sz] = {"", "rename", "-p", "-m", "KD-5", "-i"};
        // int sz = 3;
        // char* gg[sz] = {"", "generate", "200"};
        int sz = 5;
        //char* gg[sz] = {"", "getUsers", "-i"};
        char* gg[sz] = {"", "getResults", "comp.txt", "-p", "robox8"};
        core.run(sz, gg);
    }
    else
    {
        core.run(argc, argv);
    }
    
    //core.getResults("Гиды-переводчики 2022 Французский");

    return 0;
}

//--------------------------------------------------------------------------------
