#include <iostream>
#include <string>
#include <vector>

#include "dice.h"
#include "cli_args.h"

#include "tests.h"

int main(int argc, char *argv[])
{
	// Tests::test_input_recognition();
	CLI::ArgumentViews arg_views = CLI::generate_argument_views(argc, argv);
	CLI::ArgumentTokens arg_tokens = CLI::tokenise_arguments(arg_views);

	for(const auto& arg : arg_views)
	{
		std::cout << arg << '\n';
	}

	for(const auto& token : arg_tokens)
	{
		std::cout << (CLI::TokenID)token << '\n';
	}

	return 0;
}
