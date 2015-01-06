#include <iostream>
#include <fstream>
#include <string>
#include "Parser.h"

int main(int argc, char * argv[])
{
	// std::cout << "CSV Parser version 0.1" << std::endl;
	
	std::string filename("Workbook1.csv");
	if (argc > 1)
	{
		filename.assign(argv[1]);
	}
	else 
	{
		std::cout << "Provide .csv file to parse. Usage: [./Parser Workbook1.csv]\n--------------" << std::endl;
	}
	
	std::ifstream csvFile(filename.c_str());
	if (not csvFile.is_open())
	{
		std::cout << "File couldn't open." << std::endl;
		return 1;
	}
	Parser parser(csvFile);

	std::string field = parser.nextField();
	while (field != "_EOF_")
	{
		std::cout << field << std::endl;
		field = parser.nextField();
	}

	return 0;
}
