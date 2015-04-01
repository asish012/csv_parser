#include "Parser.h"
#include <iostream>
#include <cstdio>

#define END "THE_END"
const char separator = ',';

Parser::Parser(std::istream & input_stream) : line_(""), submission_(input_stream), currentLine_(0), charPosition_(0), state_(isSeparator)
{
	ss_ << submission_.rdbuf();
	line_ = ss_.str();

	if (line_[line_.size() - 1] == '\r') {
		line_[line_.size() - 1] = '\n';
	}
	if (line_[line_.size() -1] != '\n') {
		line_.append("\n");
	}

	// std::cout << "line_: " << line_ << std::endl;

	// parse();
}

Parser::Parser(std::string input) : line_(""), ss_(input), submission_(ss_), currentLine_(0), charPosition_(0), state_(isSeparator)
{
	// parse();
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
		// std::cout << "@:" << c << std::endl;
		switch(state_)
		{
			case isSeparator:
				if (c == '\"') {
					state_ = isSuperString;
				}
				else if (c == separator) {
					return "";
				}
				else if (c == '\n') {
					// TODO: Return row vector.
					++currentLine_;
					return "";
				}
				else {
					// c is alpha neumeric or space character
					field += c;
					state_ = isString;
				}
				break;
			case isString:
				if (c == separator) {
					state_ = isSeparator;
					return field;
				}
				else if (c == '\n') {
					// TODO: Return row vector.
					++currentLine_;
					state_ = isSeparator;
					return field;
				}
				else /*if (c != '\n') */{
					// c is alpha neumeric or space character
					field += c;
				}
				break;
			case isSuperString:
				if (charPosition_ >= line_.size()) {
					throw "Invalid CSV File";
				}
				else if (c == '\"') {
					state_ = isQuoteEnd;
				}
				else if (c == '\n')
				{
					/* replace line-break with a space-character within super string */
					field += " ";
				}
				else /*if (c != '\n')*/ {
					// c can be anything except \n (alpha,numeric,space,comma ...)
					field += c;
					state_ = isSuperString;
				}
				break;
			case isQuoteEnd:
				if (c == '\"') {
					field += '\"';
					state_ = isSuperString;
				}
				else if (c == separator) {
					state_ = isSeparator;
					return field;
				}
				else if (c == '\n') {
					// TODO: Return row vector.
					++currentLine_;
					state_ = isSeparator;
					return field;
				}
				else {
					throw "Invalid CSV File";
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
	// if (std::getline(submission_, line_)) {
		line_ = ss_.str();

		++currentLine_;
		charPosition_ = 0;

		return true;
	// }
	// return false;
}

char Parser::nextChar()
{
	if (charPosition_ >= line_.size()) {
		return 0;
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

