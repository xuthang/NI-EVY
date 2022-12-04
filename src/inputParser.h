#pragma once
#include <bits/stdc++.h>

enum class CODE
{
	SAA,
	SASS,
	SAS,
	NOTHING,
	TESTS
};

CODE parseInput(int argc, char *argv[], istream &in)
{
	if (argc < 2)
	{
		cerr << "use ./[program name] --help" << endl;
		exit(1);
	}
	string option = string(argv[1]);
	if (option == "--help")
	{
		cout << "use ./[program name] --(SAA | SASS | SAS | TEST)" << endl;
		cout << "afterwards the program will expect the string on input" << endl;
		cout << "\tSAA will print suffix array of the input" << endl;
		cout << "\tSASS will do SA-Simple-Search" << endl;
		cout << "\tSAS will do SA--Search" << endl;
		cout << "\tTEST will start Tests" << endl;
		return CODE::NOTHING;
	}
	else if (option == "--SAA")
		return CODE::SAA;
	else if (option == "--SASS")
		return CODE::SASS;
	else if (option == "--SAS")
		return CODE::SAS;
	else if (option == "--TEST")
		return CODE::TESTS;
	else
	{
		cerr << "option not recognized" << endl;
		cerr << "use ./[program name] --help" << endl;
		exit(1);
	}
}