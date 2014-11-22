#pragma once

#include <string>

namespace flint {
	
	/**
	 * Removes the code that appears between pairs of "// %flint: pause" and
	 * "// %flint: resume", so that intentionally written code, that may
	 * generate warnings, can be ignored by lint.
	 */
	std::string removeIgnoredCode(const std::string &file, const std::string &path);
};
