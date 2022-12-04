#pragma once
#include "includes.h"

auto getCompareFunction(const string &s)
{
	return [&](ll a, ll b) {
		return string_view(s.data() + a) < string_view(s.data() + b);
	};
}

void printSA(const string &s, const vll &sortedIndexes)
{
	cout << "s = " << s << ", size = " << s.size() << endl;
	cout << "Suffix Array " << endl;
	for (auto i : sortedIndexes) cout << i << " " << s.substr(i) << endl;
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

void SAA()
{
	string s;
	if(!(cin >> s))
	{
		cout << "expected input string" << endl;
		exit(2);
	}
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

void SASS()
{
	string s, p;
	if(!(cin >> s >> p))
	{
		cout << "expected input string" << endl;
		exit(2);
	}
	auto SA = getSA(s);

	const string &L = s, &x = p;
	ll n = s.size(), m = x.size();
	 
	ll d = 0, f = SA.size();
	ll res = d;
	while(d < f)
	{
		ll mid = (d+f)/2;

		ll l = lcp(p, s.substr(SA[mid])).size();
		if(l == m && l == (n - SA[mid])){
			res = mid;
			break;
		}
		if( l == (n - SA[mid])
		|| (l != m && s[SA[mid] + l] < x[l])) 
			d = mid;
		else
			f = mid;

	}

}

//-------------------------------------------------------------------

void SAS()
{
}

//-------------------------------------------------------------------
