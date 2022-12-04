#include "includes.h"
#include "inputParser.h"
#include "solver.cpp"
#include "tests.h"

int main(int argc, char *argv[])
{
	auto code = parseInput(argc, argv, cin);
	switch (code)
	{
	case CODE::SAA:
		SAA();
		break;
	case CODE::SASS:
		SASS();
		break;
	case CODE::SAS:
		SAS();
		break;
	case CODE::TESTS:
		Tests();
		break;
	default:;
	}
	return 0;
}
