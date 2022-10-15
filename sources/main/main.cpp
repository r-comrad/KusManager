#include <string>

#include "domain/error_message.hpp"

#include "path.hpp"

#include "evaluater/core.hpp"

//--------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
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
        char* gg[sz] = {"", "generateRobo", "20", "-m", "robox8"};
        core.run(sz, gg);
    }
    else
    {
        core.run(argc, argv);
    }
    
    return 0;
}

//--------------------------------------------------------------------------------
