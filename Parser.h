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

	std::vector<std::string> getHeader();
	std::vector< std::vector<std::string> > getRecords();

private:
	enum State { isSeparator, isQuoteStart, isQuoteEnd, isString, isSuperString };

	std::vector<std::string> parseLine();
	int lineNumber();
	bool readline();
	char nextChar();
	void parse();

	std::string line_;
	std::stringstream ss_;		// this attribute is only used to initialize submisison_ in 2nd constructor which takes string
	std::istream & submission_;
	int currentLine_;
	int charPosition_;
	State state_;
	std::vector< std::vector<std::string> > recordList_;
};

#endif /* INC_PARSER_H */