#include "cli_args.h"

#include <cctype>
// #include <iostream>

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

enum class DiceNotationValidationState
{
    START,
    D_JUST_FOUND,
    D_FOUND,
    MOD_FOUND,
};

bool CLI::is_dice_notation(const std::string_view& arg)
{
    using namespace CLI;

    bool is_notation_valid = false;
    DiceNotationValidationState validation_state = DiceNotationValidationState::START;

    bool should_continue = true;
    size_t char_index = 0;
    while (should_continue && char_index < arg.length())
    {
        const char c = arg.at(char_index);
        
        switch (validation_state)
        {
        case DiceNotationValidationState::START:
            if (is_d_char(c))
            {
                validation_state = DiceNotationValidationState::D_JUST_FOUND;
            }
            else if(!is_digit(c))
            {
                is_notation_valid = false;
                should_continue = false;
            }
            break;
        case DiceNotationValidationState::D_JUST_FOUND:
            if(is_digit(c))
            {
                validation_state = DiceNotationValidationState::D_FOUND;
                is_notation_valid = true;
            } 
            else
            {
                is_notation_valid = false;
                should_continue = false;
            }
            break;
        case DiceNotationValidationState::D_FOUND:
            if (is_plus(c) || is_dash(c))
            {
                validation_state = DiceNotationValidationState::MOD_FOUND;
                is_notation_valid = false;
            }
            else if(!is_digit(c))
            {
                is_notation_valid = false;
                should_continue = false;
            }
            break;
        case DiceNotationValidationState::MOD_FOUND:
            if(isdigit(c))
            {
                is_notation_valid = true;
            }
            else
            {
                is_notation_valid = false;
                should_continue = false;
            }
            break;
        }
        char_index++;
    }

    return is_notation_valid;
}
