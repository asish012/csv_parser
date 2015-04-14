#include "Parser.h"
#include <iostream>
#include <cstdio>

const char separator = ',';

Parser::Parser(std::istream & input_stream) : line_(""), submission_(input_stream), currentLine_(0), charPosition_(0), state_(isSeparator)
{
	parse();
}

Parser::Parser(std::string input) : line_(""), ss_(input), submission_(ss_), currentLine_(0), charPosition_(0), state_(isSeparator)
{
	parse();
}

int Parser::lineNumber()
{
	return currentLine_;
}

std::vector<std::string> Parser::parseLine()
{
	std::vector<std::string> result;
	std::string field;
	while (char c = nextChar())
	{
		switch(state_)
		{
			case isSeparator:
				if (c == '\"') {
					state_ = isSuperString;
				}
				else if (c == separator) {
					result.push_back("");
				}
				else if (charPosition_ >= line_.size()) {
					// end of record
					result.push_back("");
				}
				else if (c != '\n') {
					// here c is Alpha neumeric or Space char
					field += c;
					state_ = isString;
				}
				break;
			case isString:
				if (c == separator) {
					state_ = isSeparator;
					result.push_back(field);
					field = "";
					if (charPosition_ >= line_.size()) {
						// line ends with a comma means an empty attribute at the end
						result.push_back("");
					}
				}
				else if (charPosition_ >= line_.size()) {
					// end of line, charPosition points one step ahead of c, so accumulate c and finalize attribute
					field += c;
					state_ = isSeparator;
					result.push_back(field);
					field = "";
				}
				else if (c != '\n') {
					field += c;
				}
				break;
			case isSuperString:
				if (c == '\"') {
					state_ = isQuoteEnd;
					if (charPosition_ >= line_.size()) {
						// end of record
						state_ = isSeparator;
						result.push_back(field);
						field = "";
					}
				}
				else if (c != '\n') {
					field += c;
					state_ = isSuperString;
				}
				break;
			case isQuoteEnd:
				if (c == '\"') {
					field += '\"';
					state_ = isSuperString;
				}
				else if (c == separator or c == '\n') {
					state_ = isSeparator;
					result.push_back(field);
					field = "";
				}
				break;
			default:
				break;
		}
	}
	return result;
}

bool Parser::readline()
{
	if (std::getline(submission_, line_)) {
		int n = std::count(line_.begin(), line_.end(), '\"');
		// if number of double-cote character is odd then read line_ until it gets even (which indicates the end of superString)
		while (n > 0 and n % 2 != 0) {
			std::string str;
			if (not std::getline(submission_, str)) {
				// break;
				throw "Invalid CSV File.";
			}
			line_ += str;
			n = std::count(line_.begin(), line_.end(), '\"');
		}

		++currentLine_;
		charPosition_ = 0;
		int len = line_.size();
		if (line_[len - 1] == '\r') {
			line_[len - 1] = '\n';
		}
		return true;
	}
	return false;
}

char Parser::nextChar()
{
	if (charPosition_ >= line_.size()) {
		// if (not readline()) {
			return 0;
		// }
	}
	return line_[charPosition_++];
}

// NOTE: Line counting for a csv file starts with 1. Parser constructor initializes currentLine_ with 0
//       and when parseLine() asks for reading first line it incriments currentLine_.
void Parser::parse()
{
	while (readline())
	{
		std::vector<std::string> row = parseLine();
		recordList_.push_back(row);
	}
}

std::vector<std::string> Parser::getHeader()
{
	return recordList_[0];
}

std::vector< std::vector<std::string> > Parser::getRecords()
{
	return recordList_;
}

