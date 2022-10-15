#ifndef CODE_INFO_HPP
#define CODE_INFO_HPP

//--------------------------------------------------------------------------------

/*
** The following structures and class manage code file path,
** information about its language and 
** the desired path for the executable file.
*/

//--------------------------------------------------------------------------------

#include <memory.h>
#include <optional>

#include "domain/string.hpp"
#include "domain/error_message.hpp"

//--------------------------------------------------------------------------------

namespace test
{
    /*
    ** Structures for C++ code files.
    */
    struct CPPInfo
    {
        dom::CharArray inputFileName;
        dom::CharArray outputFileileName;
        dom::CharArray compilerName;
    };

    /*
    ** Structures for Python code files.
    */
    struct PythonInfo
    {
        dom::CharArray fileName;
        dom::CharArray interpretatorName;
    };

    /*
    ** Base class for code files.
    */
    class CodeInfo
    {
    public:
        enum class CodeInfoType { Submission, Checker };
        
        CodeInfo(CodeInfoType aType) noexcept;
        ~CodeInfo() = default;

        CodeInfo(const CodeInfo& other) = delete;
        CodeInfo& operator= (const CodeInfo& other) = delete;

        CodeInfo(CodeInfo&& other) noexcept = default;
        CodeInfo& operator= (CodeInfo&& other) noexcept = default;


        /*
        \brief Create a CPPInfo structure for the C++ file
            by moving data from CodeInfo class.
        \return CPPInfo structure.
        */
        operator CPPInfo() noexcept;

        /*
        \brief Create a PythonInfo structure for the Python file
            by moving data from CodeInfo class.
        \return PythonInfo structure.
        */
        operator PythonInfo() noexcept;

        std::string getFileLanguage() const noexcept;

        void setFileName(const dom::CharArray& aStr) noexcept;
        void setDesirableOutputFileName(const dom::CharArray& aStr) noexcept;

        /*
        \brief Set the language for the current file. 
            If the language is not specified, 
            determine the language of the file 
            from its extension.
        \param aStr The file language.
            If empty, extension of the first is used.
        */
        template 
        <
            typename S,
            typename = typename std::enable_if
            <
                std::is_same
                <
                    dom::CharArray,
                    typename std::decay<S>::type
                >::value
            >::type
        >
        void setLanguage(S&& aStr) noexcept
        {
            if (aStr.isEmpty())
            {
                setLanguage();
            }
            else
            {
                mCodeData[size_t(CellMap::Language)] = std::forward(aStr);
            }
        }

        void setLanguage() noexcept;

    private:
        dom::CharArrayTable mCodeData;
        std::optional <std::string> getFileExtension() const noexcept;

        enum class CellMap {Submission = 0, Output = 1, Language = 2};
        // First    cell            - submission file name
        // Secind   cell [optional] - output file name
        // Therd    cell [optional] - language
        // ...
    };
}

//--------------------------------------------------------------------------------

#endif // !CODE_INFO_HPP
