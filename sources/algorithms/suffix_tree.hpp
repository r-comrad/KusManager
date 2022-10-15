#ifndef SUFFIX_TREE_HPP
#define SUFFIX_TREE_HPP

//--------------------------------------------------------------------------------

#include <map>
#include <memory>
#include <string>

//--------------------------------------------------------------------------------

namespace alg
{
    class SuffixTree
    {
    public:
        SuffixTree() noexcept;
        ~SuffixTree() = default;


        void add(const std::string& aName, uint32_t aFinishNumber) noexcept;
        uint32_t get(const std::string& aName) const noexcept;

    private:
        struct Node
        {    
            Node();
            ~Node() = default;

            bool mIsFinish;
            uint32_t mFinishNumber;
            std::map<char, std::unique_ptr<Node>> mNextNodes;

            void add(const std::string& aName, uint32_t aFinishValue, size_t aCurNum = 0) noexcept;
            uint32_t get(const std::string& aName, size_t aCurNum = 0) const noexcept;
        };

        std::unique_ptr<Node> mRoot;
    };
}

//--------------------------------------------------------------------------------

#endif // !SUFFIX_TREE_HPP
