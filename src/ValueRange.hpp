#pragma once

#include <concepts>

template<std::totally_ordered T>
class ValueRange
{
private:
    T _min, _max;
public:
    ValueRange(T min, T max);
    T min() const;
    T max() const;
    bool x_in_range(T x) const;
};
