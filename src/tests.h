#pragma once
#include "solver.cpp"


void test_lcp()
{
	string a = "abcabc", b = "abcaaa";
	assert(lcp(a, b) == "abca");
}

void Tests()
{
	cout << "starting tests" << endl;
	test_lcp();
	cout << "passed all tests" << endl;
}