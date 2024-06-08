#include "dice.h"

namespace Dice
{
    Dice::Dice(Side nSides) : sides(nSides), random_number_engine(rand_device()), distribution(1, sides) {}

    Dice Dice::create_d4() { return Dice(4); }
    Dice Dice::create_d6() { return Dice(6); }
    Dice Dice::create_d8() { return Dice(8); }
    Dice Dice::create_d10(){ return Dice(10); }
    Dice Dice::create_d12(){ return Dice(12); }
    Dice Dice::create_d20(){ return Dice(20); }

    Side Dice::n_sides() const { return sides; };
    Side Dice::roll() { return distribution(random_number_engine); }

} // namespace Dice
