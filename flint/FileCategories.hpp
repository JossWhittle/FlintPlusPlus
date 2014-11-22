#pragma once

#include <string>

namespace flint {

	// Indentifiers for files marked for linting
	enum class FileCategory {
		HEADER, INL_HEADER, SOURCE_C, SOURCE_CPP, UNKNOWN,
	};

	// File identifying functions...
	FileCategory getFileCategory(const std::string &path);

	bool isHeader(const std::string &path);
	bool isSource(const std::string &path);

	std::string getFileNameBase(const std::string &path);
	std::string getFileName(const std::string &path);
};
