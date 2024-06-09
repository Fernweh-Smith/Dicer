#include "tests.h"
#include "cli_args.h"

#include <iostream>

struct DiceNotationTestData
{
    std::string input;
    bool is_valid;
};

// using DiceNotationTestResult = std::pair<DiceNotationTestData, bool>;

void test_dice_notation_recognition()
{
    const std::vector<DiceNotationTestData> test_data
    {
        {"1d20", true},
        {"12d1", true},
        {"d10", true},
        {"d10+12", true},
        {"1d20+12", true},
        {"1d20-21", true},
        
        {"3c23", false},
        {"14dx12", false},
        {"1d20+", false},
        {"1d20-", false},
        {"1d20+12x", false},
        {"1d", false},
        {"d20+1d12", false},
        {"d20+", false},
        {"d+1", false},
        {"1d+1", false},
        {"4d+11", false},
        {"4x+11", false},
        {"x1d20", false},
        {"1xd20", false},
        {"1d2v0", false},
        {"4d+1.1", false},
    };

    int longest_input = std::max_element(test_data.cbegin(), test_data.cend(), 
    [](const DiceNotationTestData& a, const DiceNotationTestData& b)
        { return a.input.length() < b.input.length(); })->input.length();

    const std::string valid = "Valid";
    const std::string invalid = "Invalid";

    std::cout << "Dice Notation Recognition Tests.\n";

    bool all_tests_passed = true;

    for (const auto &data : test_data)
    {
        const bool test_result = CLI::is_dice_notation(data.input);
        const bool expected_result = data.is_valid;
        const bool results_match = test_result == expected_result;

        std::cout << "Results Match: ";

        if(results_match)
        {
            std::cout << "Yes";
        }
        else
        {
            std::cout << "No ";
            all_tests_passed = false;
        }

        std::cout << " | Input: " << data.input << std::string(longest_input+1 - data.input.length(), ' ') << "| Expected Result: ";
        if(data.is_valid)
        {
            std::cout << valid << "  ";
        }
        else
        {
            std::cout << invalid;
        }

        std::cout << " | Test Result: ";

        if(CLI::is_dice_notation(data.input))
        {
            std::cout << valid << "  ";
        }
        else
        {
            std::cout << invalid;
        }
        std::cout << '\n';
    }
    std::cout << "All Tests Passed: " << (all_tests_passed ? "Yes" : "No") << std::endl;
}

void Tests::test_input_recognition()
{
    test_dice_notation_recognition();
}