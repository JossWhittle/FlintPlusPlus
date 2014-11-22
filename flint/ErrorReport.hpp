#pragma once

#include <iostream>
#include <string>
#include <array>
#include <vector>

#include "Polyfill.hpp"
#include "Options.hpp"

namespace flint {

	/*
	* Class to represent a single "Error" that was found during linting
	*/
	class ErrorObject {
	private:
		// Members
		const Lint m_type;
		const size_t m_line;
		const std::string m_title, m_desc;

	public:

		// Constructor
		ErrorObject(Lint type, size_t line, std::string title, std::string desc) :
			m_type(type), m_line(line), m_title(move(title)), m_desc(move(desc)) {};

		// Getter
		uint getType() const {
			return m_type;
		};

		/*
		* Prints an single error of the report in either
		* JSON or Pretty Printed format
		*
		*/
		void print(const std::string &path) const {

			static const std::array<std::string, 6> typeStr = {
				{ "[Error  ] ", "[Warning] ", "[Advice ] ", "Error", "Warning", "Advice" }
			};

			if (Options.LEVEL < m_type) {
				return;
			}

			if (Options.JSON) {
				std::cout <<	"        {\n"
					"	        \"level\"    : \"" << typeStr[m_type + 3u]  << "\",\n"
					"	        \"line\"     : "   << std::to_string(m_line)     << ",\n"
					"	        \"title\"    : \"" << escapeString(m_title) << "\",\n"
					"	        \"desc\"     : \"" << escapeString(m_desc)  << "\"\n"
					"        }";

				return;
			}

			std::cout << typeStr[m_type] << path << ':' 
				 << std::to_string(m_line) << ": " << m_title << std::endl;
		};
	};

	/*
	* Base Class for ErrorFile and ErrorReport which both have error counts
	*/
	class ErrorBase {
	protected:
		// Members
		uint m_errors, m_warnings, m_advice;
	public:
		
		ErrorBase() : m_errors(0), m_warnings(0), m_advice(0) {};

		uint getErrors() const {
			return m_errors;
		};
		uint getWarnings() const {
			return m_warnings;
		};
		uint getAdvice() const {
			return m_advice;
		};
		uint getTotal() const {
			return m_advice + m_warnings + m_errors;
		};
	};

	/*
	* Class to represent a single file's "Errors" that were found during linting
	*/
	class ErrorFile : public ErrorBase {
	private:
		// Members
		std::vector<ErrorObject> m_objs;
		const std::string m_path;

	public:

		explicit ErrorFile(std::string path) : ErrorBase(), m_path(move(path)) {};

		void addError(ErrorObject error) {
			if (error.getType() == Lint::ERROR) {
				++m_errors;
			}
			else if (error.getType() == Lint::WARNING) {
				++m_warnings;
			}
			if (error.getType() == Lint::ADVICE) {
				++m_advice;
			}
			m_objs.push_back(std::move(error));
		};

		/*
		* Prints an single file of the report in either
		* JSON or Pretty Printed format
		*/
		void print() const {

			if (Options.JSON) {
				std::cout << "    {\n"
					"	    \"path\"     : \"" << escapeString(m_path)     << "\",\n"
					"	    \"errors\"   : "   << std::to_string(getErrors())   << ",\n"
					"	    \"warnings\" : "   << std::to_string(getWarnings()) << ",\n"
					"	    \"advice\"   : "   << std::to_string(getAdvice())   << ",\n"
					"	    \"reports\"  : [\n";

				for (size_t i = 0, size = m_objs.size(); i < size; ++i) {
					if (i > 0) {
						std::cout <<  ',' << std::endl;
					}

					m_objs[i].print(m_path);
				}

				std::cout << "\n      ]\n"
						"    }";

				return;
			}
			
			for (size_t i = 0, size = m_objs.size(); i < size; ++i) {
				m_objs[i].print(m_path);
			}
		};
	};

	/*
	* Class to represent the whole report and all "Errors" that were found during linting
	*/
	class ErrorReport : public ErrorBase {
	private:
		// Members
		std::vector<ErrorFile> m_files;
	public:

		void addFile(ErrorFile file) {
			m_errors	+= file.getErrors();
			m_warnings	+= file.getWarnings();
			m_advice	+= file.getAdvice();

			m_files.push_back(std::move(file));
		};

		/*
		* Prints an entire report in either 
		* JSON or Pretty Printed format
		*
		* @return
		*		Returns a string containing the report output
		*/
		void print() const {
			
			if (Options.JSON) {
				std::cout << "{\n"
					"	\"errors\"   : " << std::to_string(getErrors())   << ",\n"
					"	\"warnings\" : " << std::to_string(getWarnings()) << ",\n"
					"	\"advice\"   : " << std::to_string(getAdvice())   << ",\n"
					"	\"files\"    : [\n";

				for (size_t i = 0, size = m_files.size(); i < size; ++i) {
					if (i > 0) {
						std::cout << ',' << std::endl;
					}

					m_files[i].print();
				}

				std::cout <<	"\n  ]\n"
						"}";

				return;
			}

			for (size_t i = 0, size = m_files.size(); i < size; ++i) {
				if (m_files[i].getTotal() > 0) {
					m_files[i].print();
				}
			}

			std::cout << "\nLint Summary: " << std::to_string(m_files.size()) << " files\n"
				"Errors: " << std::to_string(getErrors());

			if (Options.LEVEL >= Lint::WARNING) {
				std::cout << " Warnings: " << std::to_string(getWarnings());
			}
			if (Options.LEVEL >= Lint::ADVICE) {
				std::cout << " Advice: " << std::to_string(getAdvice());
			}
			std::cout << std::endl;
		};
	};

};
