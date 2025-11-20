#include "../core/common.hpp"
#include <math.h>
namespace gc
{
real_type mmin(real_type value)
{
    return value;
}

template <typename... Args> real_type mmin(real_type first, Args... args)
{
    return std::min(first, mmin(args...));
}

real_type mmax(real_type value)
{
    return value;
}

template <typename... Args> real_type mmax(real_type first, Args... args)
{
    return std::max(first, mmax(args...));
}
} // namespace gc