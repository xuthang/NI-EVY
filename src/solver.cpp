#pragma once
#include "includes.h"
#include "polachRadixSort.h"

// s string that will searched
// SA is a suffix array of len s.size()

auto getCompareFunction(const string &s)
{
	return [&](ll a, ll b)
	{
		// goes until end of shorter string and tries to find first not matching pair
		FOR(i, 0, min(s.size() - a, s.size() - b)) if (s[a + i] != s[b + i]) return s[a + i] < s[b + i];

		// shorter string is prefix of the longer one, s_a < s_b iff |s_a| < |s_b|
		return s.size() - a < s.size() - b;
	};
}

void printSA(const string &s, const vll &SA, const vll &lcpArray)
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

const ll SType = 1, LType = 0;
vll getLSTypes(const string &s)
{
	vll ret(s.size());
	ret[s.size() -1] = LType;
	for(ll i = s.size()-2; i>=0; i--) ret[i] = s[i] != s[i+1]?  s[i] < s[i+1] : ret[i+1];

	FOR(i, 0, s.size()) cerr << i << "\t";
	cerr << endl;

	FOR(i, 0, s.size()) cerr << (ret[i] == SType? "S" : "L") << "\t";
	cerr << endl;

	return ret;
}

vll lms(const string &s, const vll &LSTypeA)
{
	vll ret;
	FOR(i, 1, s.size())
	{
		if(LSTypeA[i-1] == LType && LSTypeA[i] == SType) ret.push_back(i);
	}

	FOR(i, 0, ret.size()) cerr << i << " " << ret[i] << endl;
	return ret;
}

vll createSA(const string &s)
{
	auto LSTypeA = getLSTypes(s);
	auto lmsA = lms(s,LSTypeA);

	// todo: make linear
	vll suffixes(s.size());
	iota(suffixes.begin(), suffixes.end(), 0);
	auto cmp = getCompareFunction(s); //takes index of starting position of suffix
	sort(suffixes.begin(), suffixes.end(), cmp);
	return suffixes;
}

//-------------------------------------------------------------------

string lcp(const string &a, const string &b, int aStart, int bStart)
{
	int aLen = a.size() - aStart, bLen = b.size() - bStart;
	FOR(i, 0, min(aLen, bLen)) if (a[aStart + i] != b[bStart + i]) a.substr(aStart, i);

	return aLen < bLen ? a.substr(aStart) : b.substr(bStart);
}

string lcp(const string &a, const string &b) { return lcp(a, b, 0, 0); }

int LCP_len(const string &a, const string &b, int aStart, int bStart)
{
	int aLen = a.size() - aStart, bLen = b.size() - bStart;

	//returns position of 1st not matching position, iterates until the end of the shorter string
	FOR(i, 0, min(aLen, bLen)) if (a[aStart + i] != b[bStart + i]) return i;

	// one of the suffixes is prefix of the other, return the shorter one
	return min(aLen, bLen);
}

//-------------------------------------------------------------------

vll createLCP_array(const string &s, const vll &SA)
{
	ll n = s.size();
	vll lcp(n), rank(n);
	FOR(i, 0, n) rank[SA[i]] = i;

	//implementation is kasai's algorithm
	ll h = 0;
	FOR(i, 0, n)
	if (rank[i] > 0)
	{
		ll j = SA[rank[i] - 1];
		while (s[i + h] == s[j + h]) h++;
		lcp[rank[i]] = h;
		if (h > 0) h--;
	}
	// //todo: improve this
	// vll ret(n);
	// FOR(i, 1, n) ret[i] = LCP_len(s, s, SA[i-1], SA[i]);
	// assert(ret == lcp);

	lcp.resize(2*n);
	queue<ii> q;
	q.push({0, n-1});
	//extend of the 2nd half of the array to contain values of binary search tree
	while(!q.empty())
	{
		auto [lb, ub] = q.front(); q.pop();
		ll mid = (lb + ub) /2;
		lcp[n  + mid] = LCP_len(s, s, SA[lb], SA[ub]);
		if(ub - lb > 2) q.push({lb, mid}), q.push({mid, ub});
	}
	return lcp;
}


//require lb >= 0 && ub < s.size()
ll getLCP(const vll &LCPA, const string &s, ll lb, ll ub)
{
	if(lb + 1 == ub) return LCPA[ub];

	//value was precomputed
	ll n = LCPA.size() / 2 ;
	return LCPA[n + (lb + ub)/2]; //todo: indexing??
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

void SAA(const string &s)
{
	auto sa = createSA(s);
	auto lcpa = createLCP_array(s, sa);
	printSA(s, sa, lcpa);
}

//-------------------------------------------------------------------

void SASS(const string &s, const string &p)
{
	auto SA = createSA(s);

	ll lb = 0, ub = s.size() - 1;
	ll res = -1;
	bool foundExactMatch = false;
	while (lb <= ub)
	{
		ll mid = (lb + ub) / 2;
		ll ithSuffixSize = s.length() - SA[mid];
		ll l = LCP_len(p, s, 0, SA[mid]);
		if (l == (ll)p.size() && l == ithSuffixSize) // found exact match
		{
			res = mid;
			foundExactMatch = true;
			break;
		}

		if (l == ithSuffixSize								 // suffix is too short, find a longer suffix
			|| (l != (ll)p.size() && s[SA[mid] + l] < p[l])) // pattern match until l-th symbol only, check for order with next symbol
		{
			lb = mid + 1;
			res = mid;
		}
		else
		{
			ub = mid - 1;
		}
	}

	cout << "string:" << s << endl;
	cout << "pattern:" << p << endl;
	cout << res <<"-th string" << endl;
	if (foundExactMatch)
	{
		cout << "position at: ";
		cout << "(" << SA[res] << ", " << s.substr(SA[res]) << ")" << endl;
	}
	else if (res == -1)
	{
		cout << "position before: ";
		cout << "(0, " << s.substr(SA[0]) << ")" << endl;
	}
	else if (res < (ll)SA.size() - 1)
	{
		cout << "position between: ";
		cout << "(" << SA[res] << ", " << s.substr(SA[res]) << ")";
		cout << ", ";
		cout << "(" << SA[res + 1] << ", " << s.substr(SA[res + 1]) << ")";
		cout << endl;
	}
	else
	{
		cout << "position after: ";
		cout << "(" << s.size() - 1 << ", " << s.substr(SA.back()) << ")" << endl;
	}
}

//-------------------------------------------------------------------

void SAS(const string &s, const string &p)
{
	ll n = s.size(), m = p.size();
	auto SA = createSA(s);
	auto LCPA = createLCP_array(s, SA);
	ll d = 0, f = n - 1;

	ll l_d = LCP_len(p, s, 0, SA.front()), l_f = LCP_len(p, s, 0, SA.back());
	while (d + 1 < f)
	{
		ll i = (d + f) / 2;
		deb(i);
		deb(s.substr(SA[d]));
		deb(s.substr(SA[i]));
		deb(s.substr(SA[f]));
		ll lcp_if = getLCP(LCPA, s, i, f), lcp_di = getLCP(LCPA, s, d, i);
		deb(lcp_di);
		deb(lcp_if);
		if (l_d <= lcp_if && lcp_if < l_f) // half near UB has better prefix
		{
			cerr << "case a)" << endl;
			d = i, l_d = lcp_if;
		}
		else if (l_d <= l_f && l_f < lcp_if) // mid has better LCP than UB
		{
			cerr << "case b)" << endl;
			f = i;
		}
		else if (l_f <= lcp_di && lcp_di < l_d)
		{
			cerr << "case a) 2" << endl;
			f = i, l_f = lcp_di;
		}
		else if (l_f <= l_d && l_d < lcp_di)
		{
			cerr << "case b) 2" << endl;
			d = i;
		}
		else
		{
			cerr << "case c)" << endl;

			ll l = max(l_d, l_f);
			l += LCP_len(p, s, l, SA[i] + l);
			ll Li_len = n - SA[i];
			if (l == m && l == Li_len) // found exact match
			{
				d = f = i;
				break;
			}
			else if (l == Li_len || (l != m && s[SA[i] + l] < p[l]))
			{
				cerr << "c 1" << endl;
				d = i, l_d = l;
			}
			else
			{
				cerr << "c 2" << endl;
				f = i, l_f = l;
			}
		}
	}
	cout << "string: "<< s << endl;
	cout << "pattern: " << p << endl;

	cout << "found match at " << d << " " << f << endl;
	cout << "corresponding strings are:" << endl;
	cout << s.substr(SA[d]) << endl;
	cout << s.substr(SA[f]) << endl;
}
