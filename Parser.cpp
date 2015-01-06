#include "Parser.h"
#include <iostream>
#include <cstdio>

Parser::Parser(std::istream & input_stream) : submission_(input_stream), lineNumber_(0), charPosition_(0), state_(isSeperator), line_(""), seperator_(',')
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
		// printf("--- nextChar %c (%0x) [Pos: %d] state_ %d \n", c, c, (charPosition_ - 1), state_);
		switch(state_)
		{
			case isSeperator:
				if (c == '\"')
				{
					state_ = isSuperString;
				}
				else if (c == ',')
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
				if (c == seperator_ or c == '\n')
				{
					state_ = isSeperator;					
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
						state_ = isSeperator;
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
				else if (c == seperator_ or c == '\n')
				{
					state_ = isSeperator;
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
	// std::cout << "*** line_" << line_ << std::endl;
	return line_[charPosition_++];
}