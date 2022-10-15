#ifndef TEST_HPP
#define TEST_HPP

//--------------------------------------------------------------------------------

#include <map>
#include <thread>
#include <future>
#include <memory>

#include "database/database_query.hpp"

#include "process/process.hpp"

#include "testlib_message.hpp"
#include "thread_signals.hpp"

//--------------------------------------------------------------------------------

namespace test
{
    class Test
    {
    public:
        enum class TestVerdict {NUN, OK, WA, TLE, MLE, PE};

        Test(std::shared_ptr<proc::Process> aSolutionTemplate,
            std::shared_ptr<proc::Process> aCheckerTemplate,
            ThreadSignals* aThreadSignals) noexcept;
        ~Test();

        // Test(const Test& other) noexcept;
        // Test& operator=(const Test& other) noexcept;

        Test(Test&& other) noexcept = default;
        //Test& operator=(Test&& other) noexcept = default;


        Test(const Test& other) noexcept = delete;
        Test& operator=(const Test& other) noexcept = delete;

        //Test(Test&& other) noexcept = delete;
        Test& operator=(Test&& other) noexcept = delete;

        bool isFinished() const noexcept;    
        bool testsAreOver() const noexcept;

        TestVerdict getVerdict() const noexcept;
        uint64_t getUsedTime() const noexcept;
        uint64_t getUsedMemory() const noexcept;

        void setLimits(const data::Limits& aTimeMemLimits) noexcept;

        void run(data::DatabaseQuery& aDBQ) noexcept;

    private:
        proc::Process mSolutionProcess;
        proc::Process mCheckerProcess;
        std::shared_ptr<proc::Process> mSolutionTemplate;
        std::shared_ptr<proc::Process> mCheckerTemplate;

        test::ThreadSignals* mThreadSignals;
        std::thread mTestThread;
        //std::future<void> mTestThread;
        TestLibMessage mTLM;

        //TODO: error test number                
        uint64_t mTestNumber;
        const int64_t mNumberOfTester;
        static int64_t globalTestersNumber;

        TestVerdict mVerdict;
        uint64_t mUsedTime;
        uint64_t mUsedMemory;
        uint64_t mTimeLimit;
        uint64_t mMemoryLimit;

        void runTesting(data::DatabaseQuery& aDBQ) noexcept;

        void checkTest() noexcept;
        bool getTest(data::DatabaseQuery& aDBQ) noexcept;
        void resultEvoluation() noexcept;
    };
}

//--------------------------------------------------------------------------------

#endif // !TEST_HPP