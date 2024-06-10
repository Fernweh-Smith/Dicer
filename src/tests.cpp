#include "tests.h"
#include "cli_args.h"

#include <iostream>
#include <functional>

using InputTestData = std::vector<std::string>;
using InputValidationFuncSignature = std::function<bool(const std::string_view&)>;

InputTestData create_valid_dice_input_data()
{
    return std::vector<std::string> 
    {
        "1d20",
        "12d1",
        "d10",
        "d10+12",
        "1d20+12",
        "1d20-21",
    };
}

InputTestData create_invalid_dice_input_data()
{
    return std::vector<std::string>
    {
    "3c23",
    "14dx12",
    "1d20+",
    "1d20-",
    "1d20+12x",
    "1d",
    "d20+1d12",
    "d20+",
    "d+1",
    "1d+1",
    "4d+11",
    "4x+11",
    "x1d20",
    "1xd20",
    "1d2v0",
    "4d+1.1",
    };
}

InputTestData creat_valid_short_flag_inputs()
{
    return InputTestData{
        "-h",
        "-vhs",
    };
}

InputTestData creat_invalid_short_flag_inputs()
{
    return InputTestData{
        "--h",
        "-v-h-s",
        "- h",
        "-h ",
        "h-",
        " -h",
        "-h--sf",
        "-1",
        "-h1",
    };
}

InputTestData creat_valid_long_flag_inputs()
{
    return InputTestData{
        "--h",
        "--hello",
        "--hello-there",
        "--hello-there--general-kenobi",
    };
}

InputTestData creat_invalid_long_flag_inputs()
{
    return InputTestData{
        "-h",
        "-vhs",
        "-v-h-s",
        "- h",
        "-h ",
        "h-",
        " -h",
        "-h--sf",
        "--",
        "--hello123",
        "--hello there",
    };
}

size_t get_longest_input(const InputTestData& test_data)
{
    const auto start_iter = test_data.cbegin();
    const auto end_iter = test_data.cend();
    const auto compare_func = [](const std::string& a, const std::string& b){ return a.length() < b.length(); };
    const auto element_iter =  std::max_element(start_iter, end_iter, compare_func);
    return element_iter->length();
}

bool validate_input_data(const InputTestData& inputs, 
    InputValidationFuncSignature tester_function,
    const bool inputs_valid)
{
    size_t max_input_length = get_longest_input(inputs);

    const size_t valid_msg_pad = 2;
    const std::string msg_valid   = "Valid";
    const std::string msg_invalid = "Invalid";
    const std::string msg_passed  = "Passed";
    const std::string msg_failed  = "Failed";
    const std::string msg_separator  = " | ";

    bool all_tests_match = true;

    for (const auto &input : inputs)
    {
        bool test_says_valid = tester_function(input);
        bool results_match = test_says_valid == inputs_valid;

        if(!results_match) { all_tests_match = false; }

        results_match ? (std::cout << msg_passed) : (std::cout << msg_failed);
        std::cout << msg_separator;
        std::cout << input << std::string(max_input_length - input.length(), ' ');
        std::cout << msg_separator;
        std::cout << "Expected: ";
        inputs_valid ? (std::cout << msg_valid << std::string(valid_msg_pad, ' ')) : (std::cout << msg_invalid);
        std::cout << msg_separator;
        std::cout << "Result: ";
        test_says_valid ? (std::cout << msg_valid << std::string(valid_msg_pad, ' ')) : (std::cout << msg_invalid);
        std::cout << '\n';
    }

    return all_tests_match;
}

void test_input_validation(const InputTestData& valid_inputs,
    const InputTestData& invalid_inputs,
    const InputValidationFuncSignature& validation_function,
    const std::string& title)
{
    std::cout << title << '\n';
    std::cout << "Valid Inputs:\n";
    bool all_valid_inputs_passed = validate_input_data(
        valid_inputs,
        validation_function,
        true
    );
    std::cout << "Invalid Inputs:\n";
    bool all_invalid_inputs_passed = validate_input_data(
        invalid_inputs,
        validation_function,
        false
    );
    const bool all_tests_passed = all_valid_inputs_passed && all_invalid_inputs_passed;
    all_tests_passed ? (std::cout << "All tests passed!") : (std::cout << "Some tests failed.");
    std::cout << '\n' << std::endl;
}

void test_dice_notation_recognition()
{
    test_input_validation(
        create_valid_dice_input_data(),
        create_invalid_dice_input_data(),
        CLI::is_dice_notation,
        "Dice Notation Validation Tests"
    );
}

void test_short_flag_recognition()
{
    test_input_validation(
        creat_valid_short_flag_inputs(),
        creat_invalid_short_flag_inputs(),
        CLI::is_short_flag,
        "Short Flag Validation Tests"
    );
}

void test_long_flag_recognition()
{
    test_input_validation(
        creat_valid_long_flag_inputs(),
        creat_invalid_long_flag_inputs(),
        CLI::is_long_flag,
        "Long Flag Validation Tests"
    );
}

void Tests::test_input_recognition()
{
    test_dice_notation_recognition();
    test_short_flag_recognition();
    test_long_flag_recognition();
}