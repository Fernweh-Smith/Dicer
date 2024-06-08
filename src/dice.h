#pragma once

#include <array>
#include <random>

namespace Dice
{
    using Side = size_t;
    class Dice
    {
    private:
        explicit Dice(Side sides);
        Side sides;

        std::random_device rand_device;
        std::default_random_engine random_number_engine;
        std::uniform_int_distribution<int> distribution;
        
    public:
        static constexpr auto LegalSideValues = 
            std::to_array<Side>({4,6,8,10,12,20});
        static Dice create_d4();
        static Dice create_d6();
        static Dice create_d8();
        static Dice create_d10();
        static Dice create_d12();
        static Dice create_d20();

        Side n_sides() const;
        Side roll();
    };
} // namespace Dice
