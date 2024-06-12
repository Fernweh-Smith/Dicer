#include "commands.hpp"
#include "cli_args.hpp"
#include "dice.hpp"

#include "ValueRange.hpp"

#include <iostream>

std::string modifer_string(const int modifier)
{
    if(modifier < 0) { return std::to_string(modifier); }
    if(modifier > 0) { return "+" + std::to_string(modifier); }
    return "";
}

Commands::ReturnCodes single_dice_roll(Dice::Dice dice, 
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

Commands::ReturnCodes multi_dice_roll(Dice::Dice dice, 
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
            std::cout << "Roll " << i+1 << ": ";
            std::cout << rolls.at(i) << " | ";
            mod_str.empty() ? (std::cout << "+0") : (std::cout << mod_str);
            std::cout << " = " << rolls.at(i) + modifier << '\n';
        }
    }
    else
    {
        std::cout << "Max: " << *(std::max_element(
            rolls.cbegin(), rolls.cend())) + modifier << '\n';
        std::cout << "Min: " << *(std::min_element(
            rolls.cbegin(), rolls.cend())) + modifier << '\n';
    }
    
    return Commands::ReturnCodes::SUCCESS;
}

Commands::ReturnCodes multi_dice_roll_summed(Dice::Dice dice, 
    const size_t count, const int modifier, const bool verbose_output)
{
    std::vector<int> rolls;
    rolls.reserve(count);

    int roll_sum = 0;
    for (size_t i = 0; i < count; i++)
    {
        rolls.emplace_back(dice.roll());
        roll_sum += rolls.back();
    }

    

    if (verbose_output)
    {
        std::string mod_str = modifer_string(modifier);
        std::cout << count << "d" << dice.n_sides() << "Summed " << mod_str << '\n';
        for (size_t i = 0; i < count; i++)
        {
            std::cout << "Roll " << i+1 << ": " << rolls.at(i);
        }
        std::cout << "Sum of Rulls: " << roll_sum;
        std::cout << " | ";
        mod_str.empty() ? (std::cout << "+0") : (std::cout << mod_str);
        std::cout << " = " << roll_sum + modifier << '\n';
    }
    else
    {
        std::cout << roll_sum + modifier;
    }

    return Commands::ReturnCodes::SUCCESS;
}

Commands::ReturnCodes display_help()
{
    std::cout << "Dicer.\n";
    std::cout << '\n';
    CLI::print_usage();
    std::cout << '\n';
    CLI::print_dice_guide();
    std::cout << '\n';

    CLI::print_options();
    std::cout << '\n';

    return Commands::ReturnCodes::HELP;
}

Commands::ReturnCodes display_error_bad_args()
{
    std::cerr << "Error: Bad Arguments." << '\n';
    CLI::print_usage();
    return Commands::ReturnCodes::ERROR;
}

Commands::ReturnCodes display_error_unforseen()
{
    std::cerr << "Error: Something went wrong." << '\n';
    CLI::print_usage();
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
    else
    {
        dice_roll_data.count = 1;
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
        return { display_help };
    }

    if (arg_tokens.at(1) != CLI::Tokens::A_DICE)
    {
        return { display_error_bad_args };
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
            return { display_error_bad_args };
        }
        if (token == CLI::Tokens::F_VERBOSE){ verbose_output = true; }
        if (token == CLI::Tokens::F_SUM){ sum_multiple = true; } 
    }
    
    DiceRollData data = parse_dice_notation(arg_views.at(1));

    if(!Dice::Dice::LegalCountRange.x_in_range(data.count))
    { return { display_error_bad_args }; }

    if(!Dice::Dice::LegalModifierRange.x_in_range(data.modifier))
    { return { display_error_bad_args }; }

    std::function<Dice::Dice()> diceFactory;
    const auto factory_iter = Dice::Dice::SideFactoryMap.find(data.sides);
    if (factory_iter == Dice::Dice::SideFactoryMap.cend())
    {
        return { display_error_bad_args };
    }
    else
    {
        diceFactory = factory_iter->second;
    }

    if (data.count == 1)
    {
        return [diceFactory, data, verbose_output]()
            { return single_dice_roll(diceFactory(), data.modifier, verbose_output); };
    }
    else if (sum_multiple)
    {
        return [diceFactory, data, verbose_output]()
            { return multi_dice_roll_summed(diceFactory(),
                data.count, data.modifier, verbose_output); };
    }
    else
    {
        return [diceFactory, data, verbose_output]()
            { return multi_dice_roll(diceFactory(),
                data.count, data.modifier, verbose_output); };
    }
    
    
    return { display_error_unforseen };
}