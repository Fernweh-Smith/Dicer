#include <iostream>

int main(int argc, char** argv)
{
	std::cout << "Hello There!\n";
	std::cout << "Hello Again!\n";
	for(int i=0; i < argc; i++)
	{
		std::cout << argv[i] << '\n';
	}
	return 0;
}
