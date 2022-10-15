#include "test.hpp"

//--------------------------------------------------------------------------------

#include "domain/error_message.hpp"

//--------------------------------------------------------------------------------

int64_t test::Test::globalTestersNumber = 0;

//--------------------------------------------------------------------------------

test::Test::Test
(
    std::shared_ptr<proc::Process> aSolutionTemplate,
    std::shared_ptr<proc::Process> aCheckerTemplate,
    ThreadSignals* aThreadSignals
) noexcept :
    mNumberOfTester     (globalTestersNumber++),
    mTestNumber         (0),
    mSolutionTemplate   (aSolutionTemplate),
    mCheckerTemplate    (aCheckerTemplate),
    mThreadSignals      (aThreadSignals),
    mVerdict            (TestVerdict::OK),
    mUsedTime           (0),
    mUsedMemory         (0),
    mTestThread         ([](){})
{
mThreadSignals->push(mNumberOfTester);  
}

//--------------------------------------------------------------------------------

test::Test::~Test() 
{
    mTestThread.join();
    // mTestThread.get();
}

//--------------------------------------------------------------------------------

// test::Test::Test(const Test& other) noexcept :
//     mNumberOfTester     (globalTestersNumber++),
//     mTestNumber         (0),
//     mSolutionTemplate   (other.mSolutionTemplate),
//     mCheckerTemplate    (other.mCheckerTemplate),
//     mThreadSignals      (other.mThreadSignals),
//     mVerdict            (TestVerdict::OK),
//     mUsedTime           (0),
//     mUsedMemory         (0),
//     mTestThread         ([](){})
// {
//     // mTestThread = std::async(std::launch::async, [](){});
//     mTimeLimit = other.mTimeLimit;    
//     mMemoryLimit = other.mMemoryLimit; 
//     mThreadSignals->push(mNumberOfTester);    
// }

// //--------------------------------------------------------------------------------

// test::Test& 
// test::Test::operator=(const Test& other) noexcept
// {
//     // mTestThread = std::async(std::launch::async, [](){});
//     *this = std::move(Test(other));
//     return *this;
// }

//--------------------------------------------------------------------------------

test::Test::TestVerdict
test::Test::getVerdict() const noexcept
{
    return mVerdict;
}

//--------------------------------------------------------------------------------

uint64_t
test::Test::getUsedTime() const noexcept
{
    return mUsedTime;
}

//--------------------------------------------------------------------------------

uint64_t
test::Test::getUsedMemory() const noexcept
{
    return mUsedMemory;
}

//--------------------------------------------------------------------------------

void 
test::Test::setLimits(const data::Limits& aTimeMemLimits) noexcept
{
    mTimeLimit = aTimeMemLimits.timeLimit;
    mMemoryLimit = aTimeMemLimits.memoryLimit;
}

//--------------------------------------------------------------------------------

void 
test::Test::run(data::DatabaseQuery& aDBQ) noexcept
{
    WRITE_LOG("Entering_test", "Test_cell_num:", mNumberOfTester);
    mThreadSignals->pop(mNumberOfTester);
    mTestThread.join();
    mTestThread = std::thread(&Test::runTesting, this, std::ref(aDBQ));
    // mTestThread.get();
    // mTestThread = std::async(std::launch::async, &Test::runTesting, this, std::ref(aDBQ));
}

//--------------------------------------------------------------------------------

void 
test::Test::runTesting(data::DatabaseQuery& aDBQ) noexcept
{
    START_LOG_BLOCK("Start_new_test", "Test_cell_num:", mNumberOfTester);

    if (getTest(aDBQ)) 
    {
        //WRITE_LOG("Success", "Test_cell_num:", mNumberOfTester);
        checkTest();
        resultEvoluation();
        mThreadSignals->push(mNumberOfTester);
        //WRITE_LOG("Success_finishing", "Test_cell_num:", mNumberOfTester);
    }
    else
    {
        WRITE_LOG("No_tests_left");
        //WRITE_LOG("Failure", "Test_cell_num:", mNumberOfTester);
        mThreadSignals->finishCurrentThread();
        //WRITE_LOG("Failure_finishing", "Test_cell_num:", mNumberOfTester);
    }

    END_LOG_BLOCK("End_test#", mTestNumber, "Test_cell_num:", mNumberOfTester);
}

//--------------------------------------------------------------------------------

void 
test::Test::checkTest() noexcept
{
    START_LOG_BLOCK("Checking_test#", mTestNumber, 
        "Test_cell_num:", mNumberOfTester);

    mSolutionProcess = *mSolutionTemplate;
    mSolutionProcess.create();
    WRITE_LOG("Test_cell_num:", mNumberOfTester, "Write_data:",  mTLM.mTest);
    mSolutionProcess.writeData(mTLM.mTest);
    mSolutionProcess.writeData("\n");
    auto testRes = mSolutionProcess.runWithLimits();

    if (!testRes.has_value())
    {
        WRITE_LOG       ("Test_cell_num:", mNumberOfTester, "Output:", mTLM.mOutput);
        mVerdict = TestVerdict::TLE;
    }
    else
    {
        mUsedTime = testRes.value().first;
        mUsedMemory = testRes.value().second;

        mSolutionProcess.readData(mTLM.mOutput);
        mTLM.makeOutputSizes();

        START_LOG_BLOCK ("Test_cell_num:", mNumberOfTester, "Test:",   mTLM.mTest);
        WRITE_LOG       ("Test_cell_num:", mNumberOfTester, "Output:", mTLM.mOutput);
        END_LOG_BLOCK   ("Test_cell_num:", mNumberOfTester, "Answer:", mTLM.mAnswer);
        END_LOG_BLOCK();

        mCheckerProcess = *mCheckerTemplate;
        mCheckerProcess.create();
        mCheckerProcess.writeData(mTLM.mTestSize);
        mCheckerProcess.writeData(mTLM.mTest);

        mCheckerProcess.writeData(mTLM.mOutputSize);
        mCheckerProcess.writeData(mTLM.mOutput);

        mCheckerProcess.writeData(mTLM.mAnswerSize);
        mCheckerProcess.writeData(mTLM.mAnswer);

        mCheckerProcess.run();

        std::string temp;
    }
}

//--------------------------------------------------------------------------------

bool 
test::Test::getTest(data::DatabaseQuery& aDBQ) noexcept
{
    bool result = false;
    auto testData = aDBQ.getNextTest();
    if (testData.has_value())
    {
        mTLM.mTest = std::move(testData.value().input);
        mTLM.mAnswer = std::move(testData.value().output);   
        mTLM.makeTestSizes();
        mTLM.makeAnswerSizes();
        mTestNumber = testData.value().testNum;
        result = true;
    }
    return result;
}

//--------------------------------------------------------------------------------

void
test::Test::resultEvoluation() noexcept
{
    START_LOG_BLOCK("Test_cell_num:", mNumberOfTester, "Result_evoluation");
    WRITE_LOG("time:",   mUsedTime);
    WRITE_LOG("memory:", mUsedMemory);

    std::string temp;

    mCheckerProcess.readData(temp);

    if (temp.substr(0, 2) != "ok")
    {
        mVerdict = TestVerdict::WA;
        START_LOG_BLOCK("Result_not_okay", "Test_cell_num:", mNumberOfTester);
        END_LOG_BLOCK("Checker_output:", temp);
    }
    else if (mUsedTime > mTimeLimit * 1000000)
    {
        mVerdict = TestVerdict::TLE;
        START_LOG_BLOCK("Result_is_TLE", "Test_cell_num:", mNumberOfTester);
        WRITE_LOG("wanted:", mUsedTime);
        END_LOG_BLOCK("received:", mTimeLimit);
    }
    else if (mUsedMemory > mMemoryLimit)
    {
        mVerdict = TestVerdict::MLE;
        START_LOG_BLOCK("Result_is_MLE", "Test_cell_num:", mNumberOfTester);
        WRITE_LOG("wanted:", mUsedMemory);
        END_LOG_BLOCK("received:", mMemoryLimit);
    }
    else
    {
        mVerdict = TestVerdict::OK;
        START_LOG_BLOCK("ok_test_passed", "Test_cell_num:", mNumberOfTester);
        END_LOG_BLOCK();
    }

    END_LOG_BLOCK();
}

//--------------------------------------------------------------------------------
