#include "suffix_tree.hpp"

//--------------------------------------------------------------------------------

alg::SuffixTree::SuffixTree() noexcept : mRoot(std::make_unique<Node>())
{
}

//--------------------------------------------------------------------------------

void
alg::SuffixTree::add(const std::string& aName, uint32_t aFinishNumber) noexcept
{
    mRoot->add(aName, aFinishNumber);
}

//--------------------------------------------------------------------------------

uint32_t
alg::SuffixTree::get(const std::string& aName) const noexcept
{
    return mRoot->get(aName);
}

//--------------------------------------------------------------------------------

alg::SuffixTree::Node::Node() : mIsFinish(false), mFinishNumber(0)
{
}

//--------------------------------------------------------------------------------

void
alg::SuffixTree::Node::add(const std::string& aName, uint32_t aFinishValue,
                           size_t aCurNum) noexcept
{
    if (aCurNum == aName.size())
    {
        mIsFinish     = true;
        mFinishNumber = aFinishValue;
        return;
    }
    if (mNextNodes[aName[aCurNum]] == nullptr)
        mNextNodes[aName[aCurNum]] = std::make_unique<Node>();
    mNextNodes[aName[aCurNum]]->add(aName, aFinishValue, aCurNum + 1);
}

//--------------------------------------------------------------------------------

uint32_t
alg::SuffixTree::Node::get(const std::string& aName,
                           size_t aCurNum) const noexcept
{
    uint32_t result = mFinishNumber;
    if (!mFinishNumber)
    {
        auto it = mNextNodes.find(aName[aCurNum]);
        result  = it->second->get(aName, aCurNum + 1);
    }
    return result;
}

//--------------------------------------------------------------------------------
