#ifndef COMPILER_HPP
#define COMPILER_HPP

//--------------------------------------------------------------------------------

/*
** The compiler class manages code files for 
** code execution (including code compilation).
*/

//--------------------------------------------------------------------------------

#include <vector>

#include "domain/string.hpp"

#include "algorithms/suffix_tree.hpp"

#include "code_info.hpp"

//--------------------------------------------------------------------------------

namespace test 
{
    class Compiler
    {
    private:
        enum class Language
        {
            NUN = 0,
            MAGIC_CPP = 1,
            SNAKE = 2
        };

    public:
        Compiler() noexcept = default;
        ~Compiler() = default;

        Compiler(const Compiler& other) noexcept = default;
        Compiler& operator= (const Compiler& other) noexcept = default;

        Compiler(Compiler&& other) noexcept = default;
        Compiler& operator= (Compiler&& other) noexcept = default;

        /*
        \brief Prepare a command that executes the child process and
            compiles the file in specified directory if necessary.
        \param aCode A struct with the path to code to execute,
            output directory and language information.
        */
        dom::CharArrayTable getExecutableCommand(CodeInfo&& aCode) const noexcept;

    private:
        /*
        ** The structure for storing languages in the suffix tree.
        */
        struct LanguageTree
        {
        public:
            alg::SuffixTree tree;

            LanguageTree();

            LanguageTree(const LanguageTree& other) = delete;
            LanguageTree& operator= (const LanguageTree& other) = delete;

            LanguageTree(LanguageTree&& other) noexcept = default;
            LanguageTree& operator= (LanguageTree&& other) noexcept = default;

            ~LanguageTree() = default;
        };
        static LanguageTree mLanguages;

        /*
        \brief Compile the cpp file and return the cmd command to execute it.
        \param aInfo A cpp file information.
        \return An array of strings with the commands for 
            exe execution execution.
        */
        dom::CharArrayTable prepareCommandForCPP(CPPInfo&& aInfo) 
            const noexcept;

        /*
        \brief Prepare the cmd command for python file interpretation.
        \param aInfo A python file information.
        \return An array of strings with the commands for python 
            file einterpretation.
        */
        dom::CharArrayTable prepareCommandForPython(CPPInfo&& aInfo) 
            const noexcept;
    };
}

//--------------------------------------------------------------------------------

#endif // !COMPILER_HPP
