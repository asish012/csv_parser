#include <iostream>
#include <fstream>
#include <string>
#include "Parser.h"

int main(int argc, char * argv[])
{
	// std::cout << "CSV Parser version 0.1" << std::endl;

	std::string filename("Workbook1.csv");
	if (argc > 1) {
		filename.assign(argv[1]);
	}
	else {
		std::cout << "Using default csf file (Workbook1.csv)" << std::endl;
		std::cout << "To provide a custom csv file, run: [./Parser Workbook1.csv]" << std::endl;
		std::cout << "--------------" << std::endl;
	}

	std::ifstream csvFile(filename.c_str());
	if (not csvFile.is_open()) {
		std::cout << "File couldn't open." << std::endl;
		return 1;
	}

	std::string test_str = "FirstName,LastName,Title,ReportsTo.Email,Birthdate,Description\n"
						   "Tom  ,Jones,Senior Director,buyer@salesforcesample.com,1940-06-07Z,\"Self-described as \"\"the top\"\" branding guru on the West Coast\"\n"
						   "Ian,Dury,Chief Imagineer,cto@salesforcesample.com,,\"World-renowned expert in fuzzy logic design.\n"
						   "Influential in technology purchases.\"";

	Parser parser(csvFile);
	// Parser parser(test_str);

	std::vector< std::vector<std::string> > records = parser.getRecords();
	for (std::vector< std::vector<std::string> >::iterator i = records.begin(); i != records.end(); ++i)
	{
		std::vector<std::string> row = *i;
		for (std::vector<std::string>::iterator j = row.begin(); j != row.end(); ++j)
		{
			std::cout << *j << ",";
		}
		std::cout << std::endl;
	}

	return 0;
}
