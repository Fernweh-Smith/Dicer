#include <iostream>
#include <string>
#include <vector>

#include "dice.h"
#include "cli_args.h"

int main(int argc, char *argv[])
{
	CLI::ArgumentViews arguments = CLI::generate_argument_views(argc, argv);
	for (auto &&arg : arguments)
	{
		std::cout << arg << '\n';
	}
	
	if(CLI::is_dice_notation(arguments.at(1)))
	{
		std::cout << arguments.at(1) << " is valid dice notation!\n";
	}
	else
	{
		std::cout << arguments.at(1) << " is not valid dice notation!\n";
	}
	
	// auto d20 = Dice::Dice::create_d20();
	// std::array<unsigned int, 20> score_count;
	// score_count.fill(0);

	// int n_rolls = 20;

	// for (size_t i = 0; i < n_rolls; i++)
	// {
	// 	auto result = d20.roll();
	// 	score_count.at(result - 1)++;
	// 	// std::cout << result << '\n';
	// }

	// float smallest_percent = 100.0f;
	// float largest_percent = 0.0f;
	// for (size_t i = 0; i < score_count.size(); i++)
	// {
	// 	int count = score_count.at(i);
	// 	float count_percentage = ((float)count / (float)n_rolls) * 100;

	// 	smallest_percent = std::min(smallest_percent, count_percentage);
	// 	largest_percent = std::max(largest_percent, count_percentage);

	// 	std::cout << i + 1 << " : " << count << " : " << count_percentage << '\n';
	// }

	// std::cout << "Smallest Percentage = " << smallest_percent << '\n';
	// std::cout << "Largest Percentage = " << largest_percent << '\n';
	// std::cout << "Percentage Delta accross " << n_rolls
	// 		  << " rolls = " << largest_percent - smallest_percent << '\n';

	return 0;
}
