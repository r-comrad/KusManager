#include "compiler.hpp"

//--------------------------------------------------------------------------------

#include "domain/path.hpp"
#include "domain/error_message.hpp"

#include "main/path.hpp"

#include "process/process.hpp"

//--------------------------------------------------------------------------------

dom::CharArrayTable
test::Compiler::getExecutableCommand(CodeInfo&& aCode) const noexcept
{
    dom::CharArrayTable result;

    auto fileLanguage = Language(mLanguages.tree.get(aCode.getFileLanguage()));

    switch (fileLanguage)
    {
    case Language::MAGIC_CPP:
        result = prepareCommandForCPP(aCode);
        break;
    
    case Language::SNAKE:
        result =  prepareCommandForPython(aCode);
        break;
    }

    return result;
}

//--------------------------------------------------------------------------------

test::Compiler::LanguageTree test::Compiler::mLanguages;

//--------------------------------------------------------------------------------

test::Compiler::LanguageTree::LanguageTree() 
{
    tree.add("cpp", uint32_t(Language::MAGIC_CPP));
    tree.add("py", uint32_t(Language::SNAKE));
}

//--------------------------------------------------------------------------------

dom::CharArrayTable
test::Compiler::prepareCommandForCPP(CPPInfo&& aInfo) const noexcept
{
    dom::CharArrayTable compileCommand;

    //TODO: Move

    #ifdef BILL_WINDOWS
    compileCommand.emplace_back(CPP_COMPILER_NAME);
    compileCommand.emplace_back(std::move(aInfo.inputFileName));    
    compileCommand.emplace_back(std::move(aInfo.outputFileileName));
    compileCommand.back() += ".exe";
    #else
    compileCommand.emplace_back("g++");
    compileCommand.emplace_back(std::move(aInfo.inputFileName));    
    compileCommand.emplace_back() += "-o";
    compileCommand.emplace_back(std::move(aInfo.outputFileileName));
    compileCommand.back() += ".exe";
    #endif

    dom::CharArrayTable result;
    result.emplace_back(compileCommand.back().getCopy());

    //TODO: compiler log output
    proc::Process compiler;
    compiler.setComand(std::move(compileCommand));
    compiler.create();
    compiler.run();
    #ifdef _DBG_
    //std::string s;
    //compiler.readData(s);
    //WRITE_LOG("Compiler_output:", s);
    #endif



    return result;
}

//--------------------------------------------------------------------------------

dom::CharArrayTable
test::Compiler::prepareCommandForPython(CPPInfo&& aInfo) const noexcept
{
    dom::CharArrayTable result;

    #if     defined(LINUS_LINUX)
    result.emplace_back("python3");
    #elif   defined(BILL_WINDOWS)
    result.emplace_back("C:\\Windows\\System32\\cmd.exe");
    result.emplace_back("/c");
    result.emplace_back("python.exe");
    #endif

    result.emplace_back(std::move(aInfo.inputFileName));
    return result;
}

//--------------------------------------------------------------------------------
