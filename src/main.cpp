#include <iostream>
#include <string>
#include <vector>

#include "dice.hpp"
#include "cli_args.hpp"

#include "tests.hpp"
#include "commands.hpp"

int main(int argc, char *argv[])
{
	// Tests::test_input_recognition();
	CLI::ArgumentViews arg_views = CLI::generate_argument_views(argc, argv);
	CLI::ArgumentTokens arg_tokens = CLI::tokenise_arguments(arg_views);

	Commands::CommandFuncSigntature commandFunc =
		Commands::command_from_cli_args(arg_tokens, arg_views);
	// for(const auto& arg : arg_views)
	// {
	// 	std::cout << arg << '\n';
	// }

	// for(const auto& token : arg_tokens)
	// {
	// 	std::cout << (CLI::TokenID)token << '\n';
	// }
	Commands::ReturnCodes return_code = commandFunc();
	return static_cast<int>(return_code);
}
