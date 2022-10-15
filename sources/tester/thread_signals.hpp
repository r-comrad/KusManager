#ifndef THREADS_SIGNALS_HPP
#define THREADS_SIGNALS_HPP

//--------------------------------------------------------------------------------

#include <vector>
#include <optional>
#include <mutex>
#include <atomic>

//--------------------------------------------------------------------------------

namespace test
{
    class ThreadSignals
    {
    public:
        ThreadSignals(size_t aThreadMaxCount) noexcept;
        ~ThreadSignals() = default;

        ThreadSignals(const ThreadSignals& other) = delete;
        ThreadSignals& operator= (const ThreadSignals& other) = delete;
        
        ThreadSignals(ThreadSignals&& other) noexcept = default;
        ThreadSignals& operator= (ThreadSignals&& other) noexcept = default;

        void push(size_t aSignal) noexcept;
        void pop(size_t aSignal) noexcept;
        void finishCurrentThread() noexcept;

        std::optional<size_t> getSignal() noexcept;

        bool isAllThreadsFinished() const noexcept;
        bool isCheckingFinalTests() const noexcept;

    private:
        std::vector<size_t> mSignals;
        mutable std::mutex mSignalMutex;

        const size_t mThreadMaxCount;
        std::atomic<size_t> mFinishedThreadCount;
        std::atomic<bool> mCheckingFinalTests;
    };
}

//--------------------------------------------------------------------------------

#endif // !THREADS_SIGNALS_HPP
