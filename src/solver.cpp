#pragma once
#include "includes.h"

auto getCompareFunction(const string &s)
{
	return [&](ll a, ll b) {
		// FOR(i, 0, min(s.size() - a, s.size() - b)) if(s[a+i] != s[b+i]) return s[a+i] < s[b+i];
		// return s.size() - a < s.size() - b;
		return string_view(s.data() + a) < string_view(s.data() + b);
	};
}

void printSA(const string &s, const vll &SA, const vll & lcpArray)
{
	cout << "Length:\t" << s.size() << endl;

	cout << "Index:\t";
	FOR(i, 0, s.size()) cout << i << " ";
	cout << endl;

	cout << "String:\t";
	FOR(i, 0, s.size()) cout << s[i] << " ";
	cout << endl;

	cout << "SA:\t";
	FOR(i, 0, s.size()) cout << SA[i] << " ";
	cout << endl;
	
	cout << "LCP:\t";
	FOR(i, 0, lcpArray.size()) cout << lcpArray[i] << " ";
	cout << endl;

	// FOR(i, 0, s.size())
	// 	cout << i<< " " <<s.substr(SA[i]) << endl;
}


//-------------------------------------------------------------------

vll createSA(const string &s)
{
	//todo: make linear
	vll suffixes(s.size());
	iota(suffixes.begin(), suffixes.end(), 0);
	auto cmp = getCompareFunction(s);
	sort(suffixes.begin(), suffixes.end(), cmp);
	return suffixes;
}

string lcp(const string &a, const string & b, int aStart, int bStart)
{
	int aLen = a.size() - aStart, bLen = b.size() - bStart;
	FOR(i, 0, min(aLen, bLen)) if(a[aStart + i] != b[bStart + i]) a.substr(aStart, i);
	return aLen < bLen? a.substr(aStart) : b.substr(bStart);
}

string lcp(const string & a, const string &b){ return lcp(a, b, 0, 0);}

int LCP_len(const string &a, const string & b, int aStart, int bStart)
{
	int aLen = a.size() - aStart, bLen = b.size() - bStart;
	FOR(i, 0, min(aLen, bLen)) if(a[aStart + i] != b[bStart + i]) return i;
	return min(aLen, bLen);
}

vll createLCP_array(const string & s, const vll & SA)
{
	vll ret(s.size() *2);
	//todo: improve this
	FOR(i, 1, s.size()) ret[i] = LCP_len(s, s, SA[i-1], SA[i]);

	queue<ii> q;
	q.push({0, s.size()-1});
	while(!q.empty())
	{
		auto [lb, ub] = q.front(); q.pop();
		ll mid = (lb + ub) /2;
		deb(mid)
		ret[s.size()  + mid] = LCP_len(s, s, SA[lb], SA[ub]);
		if(ub - lb > 2) q.push({lb, mid}), q.push({mid, ub});
	}
	return ret;
}

void SAA(const string & s)
{
	auto sa = createSA(s);
	auto lcpa = createLCP_array(s, sa);
	printSA(s, sa, lcpa);
}

//-------------------------------------------------------------------

void SASS(const string & s, const string &p)
{
	auto SA = createSA(s);

	ll lb = 0, ub = SA.size();
	ll res = lb;
	while(lb <= ub)
	{
		ll mid = (lb+ub)/2;
		ll ithSuffixSize = s.length() - SA[mid];
		ll l = LCP_len(p, s, 0, SA[mid]);
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

void LCP_extended(const string &s, ll lb, ll ub, vll & res)
{
	if(lb > ub) return;

	ll mid = (lb + ub)/2;
	res[s.size()+mid] = LCP_len(s, s, lb, ub);
	LCP_extended(s, lb, mid, res);
	LCP_extended(s, mid, ub, res);
}

vll LCP_extended(const string &s)
{
	vll ret(s.size() * 2);

	FOR(i, 1, s.size())
		ret[i] = LCP_len(s, s, i-1, i);
	LCP_extended(s, 0, s.size(), ret);
	return ret;
}

void SAS(const string & s, const string &p)
{
}

//-------------------------------------------------------------------
