#ifndef CORE_HPP
#define CORE_HPP

//--------------------------------------------------------------------------------

#include "domain/string.hpp"
#include "database/database_query.hpp"

#include "process/process.hpp"

#include "test.hpp"
#include "code_info.hpp"
#include "thread_signals.hpp"

//--------------------------------------------------------------------------------

namespace test
{
    class Core
    {
    public:
        Core(const std::string& aDatabasePath) noexcept;
        ~Core() = default;

        Core(const Core& other) noexcept = default;
        Core& operator=(const Core& other) noexcept = default;

        Core(Core&& other) noexcept = default;
        Core& operator=(Core&& other) noexcept = default;
        
        void run(int aID) noexcept;

    private:
        #if     defined(BILL_WINDOWS)
        static const uint8_t THREAD_COUNTS = 30;
        #elif   defined(LINUS_LINUX)
        static const uint8_t THREAD_COUNTS = 1;
        #endif

        test::ThreadSignals mThreadSignals;
        std::vector<Test> mTests;
        data::DatabaseQuery mDBQ;

        Test::TestVerdict mFinalVerdict;
        uint64_t mFinalTime;
        uint64_t mFinalMemory;

        std::shared_ptr<proc::Process> prepareSolutionProcess
            (data::SubmissionInfo& aSubInfo) const noexcept;
        std::shared_ptr<proc::Process> prepareCheckerProcess
            (data::SubmissionInfo& aSubInfo) const noexcept;
        
        std::shared_ptr<proc::Process> prepareFile
            (CodeInfo& aCodeInfo, data::SubmissionInfo& aSubInfo) const noexcept;
        
        void check(uint64_t aID) noexcept;

        std::string verdictTostring(const Test::TestVerdict& aVerdict) 
            const noexcept;
    };
}

//--------------------------------------------------------------------------------

#endif // !CORE_HPP
