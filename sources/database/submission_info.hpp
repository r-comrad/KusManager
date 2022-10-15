#ifndef SUBMISSION_INFO_HPP
#define SUBMISSION_INFO_HPP

//--------------------------------------------------------------------------------

#include "domain/string.hpp"

//--------------------------------------------------------------------------------

namespace data
{
    struct Limits
    {
        uint64_t timeLimit;
        uint64_t memoryLimit;
    };

    struct SubmissionInfo
    {
        int ID;
        int problemID;

        Limits timeMemLim;
        dom::CharArray solutionFileName;
        dom::CharArray checkerFileName;
    };
}

//--------------------------------------------------------------------------------

#endif // !SUBMISSION_INFO_HPP
