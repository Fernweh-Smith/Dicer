#include "ValueRange.hpp"

template<std::totally_ordered T>
ValueRange<T>::ValueRange(T min, T max) : 
        _min(std::min(min, max)),
        _max(std::max(min, max))
        {}

template<std::totally_ordered T>
T ValueRange<T>::min() const { return _min; }

template<std::totally_ordered T>
T ValueRange<T>::max() const { return _max; }

template<std::totally_ordered T>
bool ValueRange<T>::x_in_range(T x) const
{
    return _min <= x || x <= _max;
}