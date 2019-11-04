#include <iostream>

#include "CmdParser.h"

int main(int argv, char** argc) {
	ArchiveFiles::CmdParser cmdP(argv, argc);
	std::string input;

	while (!cmdP.isDone()) {
		getline(std::cin, input);
		cmdP.parse(input);
	}

	return 0;
}