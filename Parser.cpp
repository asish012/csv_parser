#include "Parser.h"
#include <iostream>
#include <cstdio>

#define END "THE_END"
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

std::string Parser::nextField()
{
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
					field = "";
					return field;
				}
				else if (c != '\n') {
					field += c;
					state_ = isString;
				}
				break;
			case isString:
				if (c == separator or c == '\n') {
					state_ = isSeparator;
					return field;
				}
				else if (charPosition_ == std::strlen(line_.c_str())) {
					/* charPosition points one step ahead of c. so accumulate c and return, because its end of line */
					field += c;
					state_ = isSeparator;
					return field;
				}
				else {
					field += c;
				}
				break;
			case isSuperString:
				if (c == '\"') {
					state_ = isQuoteEnd;
					if (charPosition_ >= line_.size()) {
						state_ = isSeparator;
						return field;
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
					return field;
				}
				break;
			default:
				break;
		}
	}
	return END;
}

bool Parser::readline()
{
	if (std::getline(submission_, line_)) {
		++currentLine_;
		charPosition_ = 0;
		unsigned int len = line_.size();
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
		if (not readline()) {
			return 0;
		}
	}
	return line_[charPosition_++];
}

// NOTE: Line counting for a csv file starts with 1. Parser constructor initializes currentLine_ with 0
//       and when nextField() asks for reading first line it incriments currentLine_.
void Parser::parse()
{
	static int marker = 0;
	++marker;

	std::vector<std::string> row;

	while (1)
	{
		std::string field = nextField();
		std::cout << "::" << field << marker << ":" << currentLine_ << std::endl;
		if (marker == currentLine_ and field != END) {
			row.push_back(field);
		}
		else
			break;
	}

	if (marker < currentLine_)	{
		recordList_.push_back(row);
		parse();
	}
}

std::vector<std::string> Parser::getHeader()
{
	return recordList_[0];
}

