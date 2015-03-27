#ifndef INC_PARSER_H
#define INC_PARSER_H

#include <string>
#include <istream>
#include <sstream>
#include <vector>

class Parser
{
public:
	Parser(std::istream & input_stream);
	Parser(std::string input);

	int lineNumber();
	std::string nextField();
	std::vector<std::string> getHeader();

	std::string line_;

private:
	std::stringstream ss_;		// this attribute is only used to initialize submisison_ in 2nd constructor which takes string
	std::istream & submission_;

	int currentLine_;
	int charPosition_;

	enum State { isSeparator, isQuoteStart, isQuoteEnd, isString, isSuperString };
	State state_;

	std::vector< std::vector<std::string> > recordList_;

	bool readline();
	char nextChar();
	void parse();
};

#endif /* INC_PARSER_H */