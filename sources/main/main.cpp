#include <fstream>
#include <string>

#include "domain/error_message.hpp"

#include "core/core.hpp"

#include "path.hpp"

//--------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    WRITE_LOG("Start_main");

    std::vector<std::string> arguments;
    arguments.reserve(argc);
    for (size_t i = 0; i < argc; ++i)
    {
        arguments.push_back(argv[i]);
    }

    eval::Core core("");

    if (arguments.size() < 2)
    {
        WRITE_ERROR("Insufficient number of arguments");
        freopen("a.out", "w", stdout);

        arguments = {""s, "getResults"s, "comp.txt"s, " "s, "robox8"s};
    }

    core.run(arguments);

    return 0;
}

//--------------------------------------------------------------------------------
