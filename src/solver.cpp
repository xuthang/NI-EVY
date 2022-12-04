#pragma once
#include "includes.h"

auto getCompareFunction(const string &s)
{
	return [&](ll a, ll b) {
		// FOR(i, 0, min(a, b))
		// 	if(s[a+i] !=  s[b+i]) return s[a+i] < s[b+i];
		// return a < b;
		
		return string_view(s.data() + a) < string_view(s.data() + b);
	};
}

void printSA(const string &s, const vll &sortedIndexes)
{
	cout << "s = " << s << ", size = " << s.size() << endl;

	cout << "Suffix Array " << endl;
	FOR(i, 0, sortedIndexes.size())
		cout << i << " " << sortedIndexes[i] << " " << s.substr(sortedIndexes[i]) << endl;
}

//-------------------------------------------------------------------

vll getSA(const string &s)
{
	vll suffixes(s.size());
	iota(suffixes.begin(), suffixes.end(), 0);
	auto cmp = getCompareFunction(s);
	sort(suffixes.begin(), suffixes.end(), cmp);
	return suffixes;
}

void SAA(const string & s)
{
	auto v = getSA(s);
	printSA(s, v);
}

//-------------------------------------------------------------------

string lcp(const string & a, const string &b)
{
	string ret;
	FOR(i, 0, min(a.size(), b.size()))
		if(a[i] == b[i]) ret+=a[i];
		else break;
	return ret;
}

string lcp(const string &a, const string & b, int aStart, int bStart)
{
	string ret;
	int aLen = a.size() - aStart, bLen = b.size() - bStart;
	FOR(i, 0, min(aLen, bLen))
		if(a[aStart + i] == b[bStart + i]) ret+=a[i];
		else break;
	return ret;
}

void SASS(const string & s, const string &p)
{
	auto SA = getSA(s);

	ll lb = 0, ub = SA.size();
	ll res = lb;
	while(lb <= ub)
	{
		ll mid = (lb+ub)/2;
		ll ithSuffixSize = s.length() - SA[mid];
		auto LCP = lcp(p, s, 0, SA[mid]); //todo: make string view
		ll l = LCP.size();
		if(l == p.size() && l == ithSuffixSize) //found match until the end of s
		{
			res = mid;
			break;
		}

		if( l == ithSuffixSize //suffix is too short, find a longer suffix
		|| (l != p.size() && s[SA[mid] + l] < p[l])) // pattern match until lth symbol only, check for order with next symbol
		{
			lb = mid+1;
			res = mid;
		}
		else
		{
			ub = mid-1;
		}
	}
	cout << "string:"  << s << endl;
	cout << "pattern:" << p << endl;
	if(res < SA.size() -1)
	{
		cout << "position between: " << SA[res] << " and " << SA[res+1]  << endl;
		cout << s.substr(SA[res]) << endl;
		cout << s.substr(SA[res +1 ]) << endl;
	}
	else
	{

	}
}

//-------------------------------------------------------------------

void SAS(const string & s, const string &p)
{
}

//-------------------------------------------------------------------
