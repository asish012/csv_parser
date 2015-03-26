#ifndef INC_PARSER_H
#define INC_PARSER_H

#include <string>
#include <istream>
#include <sstream>

class Parser
{
public:
	Parser(std::istream & input_stream);
	Parser(std::string input);

	unsigned int lineNumber();
	std::string nextField();

	std::string line_;

private:
	std::stringstream ss_;		// this attribute is only used to initialize submisison_ in 2nd constructor which takes string
	std::istream & submission_;

	unsigned int lineNumber_;
	unsigned int charPosition_;

	enum State { isSeparator, isQuoteStart, isQuoteEnd, isString, isSuperString };
	State state_;

	bool readline();
	char nextChar();
};

#endif /* INC_PARSER_H */