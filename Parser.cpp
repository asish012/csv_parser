#include "Parser.h"
#include <iostream>
#include <cstdio>

const unsigned char separator = ',';

Parser::Parser(std::istream & input_stream) : submission_(input_stream), lineNumber_(0), charPosition_(0), state_(isSeparator), line_("")
{}

unsigned int Parser::lineNumber()
{
	return lineNumber_;
}

std::string Parser::nextField()
{
	std::string field;
	while (char c = nextChar())
	{
		switch(state_)
		{
			case isSeparator:
				if (c == '\"')
				{
					state_ = isSuperString;
				}
				else if (c == separator)
				{
					field = "";
					return field;
				}
				else if (c != '\n')
				{
					field += c;
					state_ = isString;
				}
				break;
			case isString:
				if (c == separator or c == '\n')
				{
					state_ = isSeparator;					
					return field;
				}
				else
				{
					field += c;
				}
				break;
			case isSuperString:
				if (c == '\"')
				{
					state_ = isQuoteEnd;					
					if (charPosition_ >= line_.size())
					{
						state_ = isSeparator;
						return field;
					}
				}
				else if (c != '\n')
				{
					field += c;
					state_ = isSuperString;
				}
				break;
			case isQuoteEnd:
				if (c == '\"')
				{
					field += '\"';
					state_ = isSuperString;
				}
				else if (c == separator or c == '\n')
				{
					state_ = isSeparator;
					return field;
				}
				break;
			default:
				std::cout << "DEFAULT" << std::endl;
				break;
		}
	}
	return "_EOF_";
}

bool Parser::readline()
{
	if (std::getline(submission_, line_)) {
		lineNumber_++;
		charPosition_ = 0;
		unsigned int len = line_.size();
		if (line_[len - 1] == '\r')
		{
			line_[len - 1] = '\n';
		}
		return true;
	}
	return false;
}

char Parser::nextChar()
{
	if (charPosition_ >= line_.size())
	{
		if (not readline())
		{
			return 0;
		}
	}
	return line_[charPosition_++];
}