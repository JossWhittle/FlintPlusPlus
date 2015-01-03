#pragma once

#include <string>
#include <vector>

namespace flint {

	enum Lint {
		ERROR, WARNING, ADVICE
	};

	struct OptionsInfo {
		bool RECURSIVE;
		bool CMODE;
		bool JSON;
		bool VERBOSE;
		int  LEVEL;
	};
	extern OptionsInfo Options;

	void printHelp();
	void parseArgs(int argc, char *argv[], std::vector<std::string> &paths);
};
