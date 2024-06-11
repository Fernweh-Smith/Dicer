#include "commands.hpp"
#include "cli_args.hpp"
#include "dice.hpp"

#include <iostream>

std::string modifer_string(const int modifier)
{
    if(modifier < 0) { return std::to_string(modifier); }
    if(modifier > 0) { return "+" + std::to_string(modifier); }
    return "";
}

Commands::ReturnCodes single_dice_roll(Dice::Dice& dice, 
    const int modifier, const bool verbose_output)
{
    const int roll_result = dice.roll();
    const int result_modified = roll_result + modifier;

    if(verbose_output)
    {
        std::string mod_str = modifer_string(modifier);
        std::cout << "d" << dice.n_sides() << mod_str << '\n';
        std::cout << "Roll: " << roll_result << " | " ;
        mod_str.empty() ? (std::cout << "+0") : (std::cout << mod_str);
        std::cout << " = " << result_modified << '\n';
    }
    else
    {
        std::cout << result_modified << '\n';
    }

    return Commands::ReturnCodes::SUCCESS;
}

Commands::ReturnCodes multi_dice_roll(Dice::Dice& dice, 
    const size_t count, const int modifier, const bool verbose_output)
{
    std::vector<int> rolls;
    rolls.reserve(count);

    for (size_t i = 0; i < count; i++)
    {
        rolls.emplace_back(dice.roll());
    }

    if (verbose_output)
    {
        std::string mod_str = modifer_string(modifier);
        std::cout << count << "d" << dice.n_sides() << mod_str << '\n';
        for (size_t i = 0; i < count; i++)
        {
            std::cout << "Roll " << i+1 << ":";
            std::cout << rolls.at(i) << " | ";
            mod_str.empty() ? (std::cout << "+0") : (std::cout << mod_str);
            std::cout << " = " << rolls.at(i) + modifier << '\n';
        }
    }
    else
    {
        std::cout << "Max: " << *(std::max_element(
            rolls.cbegin(), rolls.cend())) + modifier;
        std::cout << "Min: " << *(std::min_element(
            rolls.cbegin(), rolls.cend())) + modifier;
    }
    
    return Commands::ReturnCodes::SUCCESS;
}

Commands::ReturnCodes multi_dice_roll_summed(Dice::Dice& dice, 
    const size_t count, const int modifier, const bool verbose_output)
{

    return Commands::ReturnCodes::SUCCESS;
}

Commands::ReturnCodes display_help()
{
    // Display usage
    // Overview of dice notation
    // Flags
    std::cerr << "HELP ME PLS!\n";
    return Commands::ReturnCodes::HELP;
}

Commands::ReturnCodes display_error(const std::string& msg)
{
    std::cerr << "You done messed up!\n";
    std::cerr << msg;
    return Commands::ReturnCodes::ERROR;
}

struct DiceRollData
{
    int count, sides, modifier;
};

DiceRollData parse_dice_notation(const std::string_view& arg)
{
    DiceRollData dice_roll_data{0,0,0};

    int d_index = arg.find('d');
    if(d_index == arg.npos){ return dice_roll_data; }
    int mod_index = arg.find('+');
    if(mod_index == arg.npos) { mod_index = arg.find('-'); }

    if (d_index != 0)
    {
        const std::string count_str = std::string(arg.substr(0,d_index));
        try
        {
            int count = std::stoi(count_str.c_str());
            dice_roll_data.count = count;
        }
        catch(const std::exception& e)
        {
            std::cerr << "Failed To Convert Count" << '\n';
        }
    }

    if(mod_index == arg.npos)
    {
        const int sides_substr_start = d_index+1;
        const int sides_substr_len = arg.size() - sides_substr_start;
        const std::string sides_str = 
            std::string(arg.substr(sides_substr_start,sides_substr_len));
        try
        {
            int sides = std::stoi(sides_str.c_str());
            dice_roll_data.sides = sides;
        }
        catch(const std::exception& e)
        {
            std::cerr << "Failed To Convert Sides" << '\n';
        }
    }
    else
    {
        const int sides_substr_start = d_index+1;
        const int mod_substr_start = mod_index;
        const int sides_substr_len = mod_index - sides_substr_start;
        const int mod_substr_len = arg.size() - mod_substr_start;
        const std::string sides_str = 
            std::string(arg.substr(sides_substr_start,sides_substr_len));
        const std::string mod_str = 
            std::string(arg.substr(mod_substr_start,mod_substr_len));

        try
        {
            int sides = std::stoi(sides_str.c_str());
            dice_roll_data.sides = sides;
            int mod = std::stoi(mod_str.c_str());
            dice_roll_data.modifier = mod;
        }
        catch(const std::exception& e)
        {
            std::cerr << "Failed To Convert Sides and Modifier" << '\n';
        }
    }


    return dice_roll_data;
}


Commands::CommandFuncSigntature Commands::command_from_cli_args(
    const CLI::ArgumentTokens& arg_tokens,
    const CLI::ArgumentViews& arg_views)
{
    if (arg_tokens.at(1) == CLI::Tokens::F_HELP)
    {
        return []() { return display_help(); };
    }

    if (arg_tokens.at(1) != CLI::Tokens::A_DICE)
    {
        return []() { 
            return display_error("First Arg must be dice notation."); 
            };
    }

    

    bool verbose_output = false;
    bool sum_multiple = false;

    for (size_t i = 2; i < arg_tokens.size(); i++)
    {
        const auto& token = arg_tokens.at(i);
        if (token == CLI::Tokens::UNIDENTIFIED |
            token == CLI::Tokens::BAD_FLAG |
            token == CLI::Tokens::F_HELP |
            token == CLI::Tokens::A_DICE)
        {
            return []() { 
            return display_error("Bad arguments. <-h|--help> for help."); 
            };
        }
        if (token == CLI::Tokens::F_VERBOSE){ verbose_output = true; }
        if (token == CLI::Tokens::F_SUM){ sum_multiple = true; } 
    }
    
    DiceRollData data = parse_dice_notation(arg_views.at(1));

    if (data.count == 0 || data.sides == 0)
    {
        return []() { 
            return display_error("Bad Dice Argument"); 
            };
    }
    
    std::cerr << "Count: " << data.count << " Sides: " << data.sides
        << " Modifier: " << data.modifier << '\n';
        

    return []() { 
            return display_error(
                "Temporary Message. Will be replaced later I promise!"); 
            };
    
}