#ifndef DOM_MATH_HPP
#define DOM_MATH_HPP

//--------------------------------------------------------------------------------

#include <string>

//--------------------------------------------------------------------------------

namespace dom
{
    class Math
    {
    public:
        static uint64_t getHash(const std::string& aStr) noexcept;
    };
}

//--------------------------------------------------------------------------------

#endif // !DOM_MATH_HPP
