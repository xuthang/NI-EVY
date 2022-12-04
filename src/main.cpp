#include "includes.h"
#include "inputParser.h"
#include "solver.cpp"
#include "tests.h"

int main(int argc, char *argv[])
{
	auto code = parseInput(argc, argv, cin);
	string s, p;
	switch (code)
	{
	case CODE::SAA:
		if(!(cin >> s))
		{
			cerr << "expected input string" << endl;
			exit(2);
		}
		SAA(s);
		break;
	case CODE::SASS:
		if(!(cin >> s >> p))
		{
			cout << "expected input string" << endl;
			exit(2);
		}
		SASS(s, p);
		break;
	case CODE::SAS:
		if(!(cin >> s >> p))
		{
			cout << "expected input string" << endl;
			exit(2);
		}
		SAS(s, p);
		break;
	case CODE::TESTS:
		Tests();
		break;
	default:;
	}
	return 0;
}
