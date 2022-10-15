#include "thread_signals.hpp"

//--------------------------------------------------------------------------------

#include "domain/error_message.hpp"

//--------------------------------------------------------------------------------

test::ThreadSignals::ThreadSignals(size_t aThreadMaxCount) noexcept :
    mThreadMaxCount      (aThreadMaxCount),
    mFinishedThreadCount (0),
    mCheckingFinalTests  (false)
{
    mSignals.reserve(aThreadMaxCount);
}

//--------------------------------------------------------------------------------

void
test::ThreadSignals::push(size_t aSignal) noexcept
{
    mSignalMutex.lock();
    mSignals.emplace_back(aSignal);
    mSignalMutex.unlock();
}

//--------------------------------------------------------------------------------

void
test::ThreadSignals::pop(size_t aSignal) noexcept
{
    mFinishedThreadCount--;
}

//--------------------------------------------------------------------------------

void
test::ThreadSignals::finishCurrentThread() noexcept
{
    mCheckingFinalTests = true;
    mFinishedThreadCount++;
}

//--------------------------------------------------------------------------------

std::optional<size_t>
test::ThreadSignals::getSignal() noexcept
{
    //WRITE_LOG("Signals_count", mSignals.size());
    std::optional<size_t> result;
    mSignalMutex.lock();
    if (mSignals.size())
    {
        result = mSignals.back();
        mSignals.pop_back();
        mFinishedThreadCount++;
        //WRITE_LOG("Signal_returned", result.value());
    }
    mSignalMutex.unlock();

    //if (result.has_value()) WRITE_LOG("Signal_returned", result.value());
    return result;
}

//--------------------------------------------------------------------------------

bool
test::ThreadSignals::isAllThreadsFinished() const noexcept
{
    return mCheckingFinalTests && mFinishedThreadCount == mThreadMaxCount;
}

//--------------------------------------------------------------------------------

bool
test::ThreadSignals::isCheckingFinalTests() const noexcept
{
    return mCheckingFinalTests;
}

//--------------------------------------------------------------------------------
