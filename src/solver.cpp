#pragma once
#include "includes.h"

// s string that will searched
// SA is a suffix array of len s.size()

auto getCompareFunction(const string &s)
{
	return [&](ll a, ll b)
	{
		FOR(i, 0, min(s.size() - a, s.size() - b)) if (s[a + i] != s[b + i]) return s[a + i] < s[b + i];
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

vll createSA(const string &s)
{
	// todo: make linear
	vll suffixes(s.size());
	iota(suffixes.begin(), suffixes.end(), 0);
	auto cmp = getCompareFunction(s);
	sort(suffixes.begin(), suffixes.end(), cmp);
	return suffixes;
}

string lcp(const string &a, const string &b, int aStart, int bStart)
{
	int aLen = a.size() - aStart, bLen = b.size() - bStart;
	FOR(i, 0, min(aLen, bLen))
	if (a[aStart + i] != b[bStart + i]) a.substr(aStart, i);

	return aLen < bLen ? a.substr(aStart) : b.substr(bStart);
}

string lcp(const string &a, const string &b) { return lcp(a, b, 0, 0); }

int LCP_len(const string &a, const string &b, int aStart, int bStart)
{
	int aLen = a.size() - aStart, bLen = b.size() - bStart;
	FOR(i, 0, min(aLen, bLen))
	if (a[aStart + i] != b[bStart + i]) return i;

	return min(aLen, bLen);
}

vll createLCP_array(const string &s, const vll &SA)
{
	ll n = s.size();
	vll lcp(n), rank(n);
	FOR(i, 0, n) rank[SA[i]] = i;

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
	// queue<ii> q;

	// q.push({0, n-1});
	// while(!q.empty())
	// {
	// 	auto [lb, ub] = q.front(); q.pop();
	// 	ll mid = (lb + ub) /2;
	// 	deb(mid)
	// 	ret[n  + mid] = LCP_len(s, s, SA[lb], SA[ub]);
	// 	if(ub - lb > 2) q.push({lb, mid}), q.push({mid, ub});
	// }
	return lcp;
}

void LCP_extended(const string &s, ll lb, ll ub, vll &res)
{
	if (lb > ub)
		return;

	ll mid = (lb + ub) / 2;
	res[s.size() + mid] = LCP_len(s, s, lb, ub);
	LCP_extended(s, lb, mid, res);
	LCP_extended(s, mid, ub, res);
}

vll LCP_extended(const string &s)
{
	vll ret(s.size() * 2);

	FOR(i, 1, s.size())
	ret[i] = LCP_len(s, s, i - 1, i);
	LCP_extended(s, 0, s.size(), ret);
	return ret;
}

ll getLCP(const vll &LCPA, const string &s, ll lb, ll ub)
{
	// if(lb + 1 == ub) return LCPA[ub];
	// ll n = LCPA.size() /2 ;
	// return LCPA[n + 1+ (lb + ub)/2]; //todo: indexing??

	auto SA = createSA(s);
	return LCP_len(s, s, SA[lb], SA[ub]);
}

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
			|| (l != (ll)p.size() && s[SA[mid] + l] < p[l])) // pattern match until lth symbol only, check for order with next symbol
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
	ll res = d;

	ll l_d = 0, l_f = 0;
	while (d + 1 < f)
	{
		ll mid = (d + f) / 2;
		deb(mid);
		ll lcp_mid_ub = getLCP(LCPA, s, mid, f), lcp_lb_mid = getLCP(LCPA, s, d, mid);

		if (l_d <= lcp_mid_ub && lcp_mid_ub < l_f) // half near UB has better prefix
			d = mid, l_d = lcp_mid_ub;
		else if (l_d <= l_f && l_f < lcp_mid_ub) // mid has better LCP than UB
			f = mid;
		else if (l_f <= lcp_lb_mid && lcp_lb_mid < l_d)
			f = mid, l_f = lcp_lb_mid;
		else if (l_f <= l_d && l_d < lcp_lb_mid)
			d = mid;
		else
		{
			ll l = max(l_d, l_f);
			l += LCP_len(p, s, l, SA[mid] + l);
			ll Li_len = n - SA[mid];
			if (l == m && l == Li_len) // found exact match
			{
				res = mid;
				break;
			}
			else if (l == Li_len || (l != m && s[SA[mid] + l] < p[l]))
				d = mid, l_d = l;
			else
				f = mid, l_f = l;
		}
	}
	deb(d);
	deb(f);

	// todo: process result
}
