#ifndef PATH_HPP
#define PATH_HPP

#include "domain/path.hpp"

//--------------------------------------------------------------------------------

#define CHINA_RESOURCES EXECUTABLE_PATH + "resources/"
#define COMPILERS       CHINA_RESOURCES + "compilers/"
#define EXAMPLES        CHINA_RESOURCES + "examples/"
#define MAEDIA          EXECUTABLE_PATH + "media/"

#define DB_PATH         EXECUTABLE_PATH   
#define DATABASE_PATH   DB_PATH         + "db.sqlite3"

#define CHECKER_PATH    CHINA_RESOURCES + "checkers/"
#define SOLUTION_PATH   CHINA_RESOURCES + "solutions/"

#undef  LOGS_PATH
#define LOGS_PATH       CHINA_RESOURCES + "logs/"

#define CPP_COMPILER_NAME   COMPILERS +   "magicCPPCompiler.cmd"

//--------------------------------------------------------------------------------

#endif // !PATH_HPP
