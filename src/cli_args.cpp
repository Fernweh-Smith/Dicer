#include "cli_args.hpp"
#include "dice.hpp"

#include <cctype>
#include <iostream>


bool is_digit (const char c) { return std::isdigit(static_cast<unsigned char>(c)); }
bool is_letter(const char c) { return std::isalpha(static_cast<unsigned char>(c)); }
bool is_dash  (const char c) { return c == '-'; }
bool is_plus  (const char c) { return c == '+'; }
bool is_d_char(const char c) { return c == 'd'; }


CLI::ArgumentViews CLI::generate_argument_views(int argc, char* argv[])
{
    using namespace CLI;

    ArgumentViews argViews;
    argViews.reserve(argc);

    for (int i = 0; i < argc; i++)
    {
        argViews.emplace_back( argv[i] );
    }

    return argViews;
}

bool CLI::is_short_flag(const std::string_view& arg)
{
    constexpr size_t min_length = 2;
    
    if(arg.length() < min_length) { return false; }
    if( !is_dash(arg.at(0)) ) {return false; }

    for (size_t i = min_length-1; i < arg.length(); i++)
    {
        const char c = arg.at(i);
        if( !is_letter(c) ) { return false; }
    }
    return true;
}

bool CLI::is_long_flag(const std::string_view& arg)
{
    using namespace CLI;

    constexpr size_t min_length = 3;

    if (arg.length() < min_length) { return false; }
    if (!arg.starts_with("--")) { return false; }

    for (size_t i = min_length-1; i < arg.length(); i++)
    {
        const char c = arg.at(i);
        if( !is_letter(c) && !is_dash(c)) { return false; }
    }
    return true;        
}

namespace DiceNotationValidation
{
    enum class State
    {
        START,
        D_JUST_FOUND,
        D_FOUND,
        MOD_JUST_FOUND,
        MOD_FOUND,
        FAILED,
    };

    State handle_start(const char c)
    {
        if (is_d_char(c)) { return State::D_JUST_FOUND; }
        if (is_digit(c)) { return State::START; }
        return State::FAILED;
    }

    State handle_d_just_found(const char c)
    {
        if (is_digit(c)) { return State::D_FOUND; }
        return State::FAILED;
    }

    State handle_d_found(const char c)
    {
        if (is_plus(c) || is_dash(c)) { return State::MOD_JUST_FOUND; }
        if (is_digit(c)) { return State::D_FOUND; }
        return State::FAILED;
    }

    State handle_mod_just_found(const char c)
    {
        if (is_digit(c)) { return State::MOD_FOUND; }
        return State::FAILED;
    }

    State handle_mod_found(const char c)
    {
        if (is_digit(c)) { return State::MOD_FOUND; }
        return State::FAILED;
    }

} // namespace DiceNotation

bool CLI::is_dice_notation(const std::string_view& arg)
{
    using namespace DiceNotationValidation;

    State validation_state = State::START;

    
    for(const char c : arg)
    {
        switch (validation_state)
        {
        case State::START:
            validation_state = handle_start(c);
            break;
        case State::D_JUST_FOUND:
            validation_state = handle_d_just_found(c);
            break;
        case State::D_FOUND:
            validation_state = handle_d_found(c);
            break;
        case State::MOD_JUST_FOUND:
            validation_state = handle_mod_just_found(c);
            break;
        case State::MOD_FOUND:
            validation_state = handle_mod_found(c);
            break;
        default:
        validation_state = State::FAILED;
            break;
        }
        if(validation_state==State::FAILED){ break; }
    }

    switch (validation_state)
    {
    case State::D_FOUND:
    case State::MOD_FOUND:
        return true;
    default:
        return false;
    }
}



CLI::ArgumentTokens CLI::tokenise_arguments(const CLI::ArgumentViews& argViews)
{
    ArgumentTokens tokens;
    tokens.reserve(10);

    for(const auto& arg : argViews)
    {
        if (is_dice_notation(arg))
        {
            tokens.emplace_back(Tokens::A_DICE);
            continue;
        }
        if(is_short_flag(arg))
        {
            for (size_t i = 1; i < arg.length(); i++)
            {
                const char c = arg.at(i);
                const auto flag_iter = kShortFlags.find(c);
                if(flag_iter==kShortFlags.cend())
                {
                    tokens.emplace_back(Tokens::BAD_FLAG);
                }
                else{
                    tokens.emplace_back(flag_iter->second);
                }
            }
            continue;
        }
        if(is_long_flag(arg))
        {
            const auto flag_stripped = arg.substr(2, arg.length()-2);
            const auto flag_iter = kLongFlags.find(std::string(flag_stripped));
            if(flag_iter==kLongFlags.cend())
            {
                tokens.emplace_back(Tokens::BAD_FLAG);
            }
            else
            {
                tokens.emplace_back(flag_iter->second);
            }
            continue;
        }
        tokens.emplace_back(Tokens::UNIDENTIFIED);
    }
    return tokens;
}



void CLI::print_usage()
{
    const std::string dicer_keyword = "dicer";
    const std::string left_padding = "  ";
    std::cout << "Usage:\n";
    std::cout << left_padding << dicer_keyword << " <DICE> [flags]" << '\n';
    std::cout << left_padding << dicer_keyword << " -h|--help" << '\n';
}



void CLI::print_options()
{
    const std::string dicer_keyword = "dicer";
    const std::string left_padding = "  ";
    std::cout << "Options:\n";
    std::cout << left_padding << "-h --help        " << left_padding << "Print Help." << '\n';
    std::cout << left_padding << "-v --verbose     " << left_padding 
        << "Print results in detailed format." << '\n';
    std::cout << left_padding << "-s --sum_multiple" << left_padding 
        << "Add the results of the rolls together before adding the modifier." << '\n';
}



void CLI::print_dice_guide()
{
    const int count_min = Dice::Dice::LegalCountRange.min();
    const int count_max = Dice::Dice::LegalCountRange.max();
    const int modifer_min = Dice::Dice::LegalModifierRange.min();
    const int modifier_max = Dice::Dice::LegalModifierRange.max();

    const std::string left_padding = "  ";

    std::cout << "Dice Notation:\n";
    std::cout << left_padding << "?(int:count)d(int:n_sides)?([+|-]int:modifier)\n";
    std::cout << left_padding << "Examples: d20, 1d4, 2d12+3, d6-6\n";
    std::cout << left_padding << "Valid Counts: " << count_min << " - " << count_max << '\n';

    std:: cout << left_padding << "Valid Side Count: " << Dice::Dice::LegalSideValues.at(0);

    for (size_t i = 1; i < Dice::Dice::LegalSideValues.size(); i++)
    {
        std::cout << ", " << Dice::Dice::LegalSideValues.at(i);
    }
    std::cout << '\n';
    
    std::cout << left_padding << "Valid Modifiers: " << modifer_min << " - " << modifier_max << '\n';

}