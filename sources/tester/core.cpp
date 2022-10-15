#include "core.hpp"

//--------------------------------------------------------------------------------

#undef max
#undef min

#include <cmath>

#include "compiler.hpp"

#include <thread>
std::atomic<int> gCnt = 0;

//--------------------------------------------------------------------------------

test::Core::Core(const std::string& aDatabasePath) noexcept :
    mDBQ            (aDatabasePath),
    mFinalVerdict   (Test::TestVerdict::OK),
    mFinalTime      (0),
    mFinalMemory    (0),
    mThreadSignals  (THREAD_COUNTS)
{
    WRITE_LOG("Creating_core");
}

//--------------------------------------------------------------------------------

void
test::Core::run(int aID) noexcept
{
    auto partInfo = mDBQ.getSubmissionInfo(aID);
    mDBQ.prepareTestsStatement(partInfo.problemID);

    auto solProc = prepareSolutionProcess(partInfo);
    auto checkProc = prepareCheckerProcess(partInfo);
    solProc->setLimits(partInfo.timeMemLim);

    //Test testTemplate(solProc, checkProc, &mThreadSignals);
    //testTemplate.setLimits(partInfo.timeMemLim);
    //mTests.resize(THREAD_COUNTS, testTemplate);
    mTests.reserve(THREAD_COUNTS);
    //Test test(solProc, checkProc, &mThreadSignals);
    for(int i = 0; i < THREAD_COUNTS; ++i)
    {

        mTests.emplace_back(solProc, checkProc, &mThreadSignals);
        mTests.back().setLimits(partInfo.timeMemLim);
    }


    //check(aID);

    //TODO: remove
    try
    {
        std::thread tt(&test::Core::check, this, aID);
        int lCnt = 2e9;
        while(true)
        {
            if (gCnt < 0) break;
            if (lCnt == gCnt)
            {
                mFinalVerdict = Test::TestVerdict::TLE;
                auto tempVerdict = verdictTostring(mFinalVerdict);
                mDBQ.writeResult(aID, tempVerdict, mFinalTime, mFinalMemory);

                WRITE_LOG     ("Final_result:", tempVerdict);
                WRITE_LOG     ("Final_time:",   mFinalTime);
                END_LOG_BLOCK ("Final_memory:", mFinalMemory);

                exit(0);
            }
            lCnt = gCnt;
            std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        }
        tt.join();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    auto tempVerdict = verdictTostring(mFinalVerdict);
    mDBQ.writeResult(aID, tempVerdict, mFinalTime, mFinalMemory);

    WRITE_LOG     ("Final_result:", tempVerdict);
    WRITE_LOG     ("Final_time:",   mFinalTime);
    END_LOG_BLOCK ("Final_memory:", mFinalMemory);
}

//--------------------------------------------------------------------------------

std::shared_ptr<proc::Process>
test::Core::prepareSolutionProcess(data::SubmissionInfo& aSubInfo) const noexcept
{
    CodeInfo codeInfo(CodeInfo::CodeInfoType::Submission);
    codeInfo.setFileName(std::move(aSubInfo.solutionFileName));
    return prepareFile(codeInfo, aSubInfo);
}

//--------------------------------------------------------------------------------

std::shared_ptr<proc::Process>
test::Core::prepareCheckerProcess(data::SubmissionInfo& aSubInfo) const noexcept
{
    CodeInfo codeInfo(CodeInfo::CodeInfoType::Checker);
    codeInfo.setFileName(std::move(aSubInfo.checkerFileName));
    return prepareFile(codeInfo, aSubInfo);

}

//--------------------------------------------------------------------------------

std::shared_ptr<proc::Process> 
test::Core::prepareFile(CodeInfo& aCodeInfo, data::SubmissionInfo& aSubInfo) 
    const noexcept
{
    std::shared_ptr<proc::Process> result = std::make_shared<proc::Process>();

    aCodeInfo.setDesirableOutputFileName(std::to_string(aSubInfo.ID));
    aCodeInfo.setLanguage();

    Compiler comp;
    auto cmd = comp.getExecutableCommand(std::move(aCodeInfo));
    result->setComand(std::move(cmd));
    return result;
}

//--------------------------------------------------------------------------------

void
test::Core::check(uint64_t aID) noexcept
{
    START_LOG_BLOCK("Checking_participant_code");

    // TODO: remove mFinalVerdict == Test::TestVerdict::OK
    while(mFinalVerdict == Test::TestVerdict::OK && 
        !mThreadSignals.isAllThreadsFinished())
    {         
        auto signal = mThreadSignals.getSignal();
        //WRITE_LOG("gg in", gCnt);
        if (signal.has_value())
        {
            //TODO: remove
            ++gCnt;
            //WRITE_LOG("inc");

            WRITE_LOG("Signal_number", signal.value());
            auto& test = mTests[signal.value()];
            auto verdict = test.getVerdict();

            if (mFinalVerdict == Test::TestVerdict::OK) 
            {
                mFinalVerdict = verdict;
            }
            mFinalTime = std::max(mFinalTime, test.getUsedTime());
            mFinalMemory = std::max(mFinalMemory, test.getUsedMemory());

            if (!mThreadSignals.isCheckingFinalTests() && 
                mFinalVerdict == Test::TestVerdict::OK) 
            {
                test.run(mDBQ);
            }
        }
        // else
        // {
        //     WRITE_LOG("NO", signal.has_value());
        // }
        //WRITE_LOG("gg out", gCnt);
    }
    
    //TODO: remove
    gCnt = -1;
}

std::string
test::Core::verdictTostring(const Test::TestVerdict& aVerdict) const noexcept
{
    std::string result;
    switch (aVerdict)
    {
    case Test::TestVerdict::OK :
        result = "ok";
        break;
    case Test::TestVerdict::WA :
        result = "wa";
        break;
    case Test::TestVerdict::TLE :
        result = "tl";
        break;
    case Test::TestVerdict::MLE :
        result = "ml";
        break;
    case Test::TestVerdict::PE :
        result = "wa";
        break;
    case Test::TestVerdict::NUN :
        result = "wa";
        break;
    }
    return result;
}

//--------------------------------------------------------------------------------
