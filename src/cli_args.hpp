#pragma once

#include <string>
#include <unordered_map>
#include <vector>
// #include <utility>

constexpr unsigned int two_power(unsigned int power)
{
    return 1 << power;
}

namespace CLI
{
    using TokenID = unsigned int;

    enum class Tokens : TokenID
    {
        UNIDENTIFIED = two_power(0),
        A_DICE       = two_power(1),
        F_HELP       = two_power(2),
        F_VERBOSE    = two_power(3),
        F_SUM        = two_power(4),
        BAD_FLAG     = two_power(5),
    };

    using ArgumentViews = std::vector<std::string_view>;
    using ArgumentTokens = std::vector<Tokens>;
    using ShortFlagMap = std::unordered_map<char, Tokens>;
    using LongFlagMap = std::unordered_map<std::string, Tokens>;

    const ShortFlagMap kShortFlags{
        {'h', Tokens::F_HELP},
        {'v', Tokens::F_VERBOSE},
        {'s', Tokens::F_SUM},
    };

    const LongFlagMap kLongFlags{
        {"help",         Tokens::F_HELP},
        {"verbose",      Tokens::F_VERBOSE},
        {"sum-multiple", Tokens::F_SUM},
    };

    ArgumentViews generate_argument_views(int argc, char* argv[]);
    ArgumentTokens tokenise_arguments(const ArgumentViews& argViews);

    bool is_short_flag(const std::string_view& arg);
    bool is_long_flag(const std::string_view& arg);
    bool is_dice_notation(const std::string_view& arg);

} // namespace cli_arguments
