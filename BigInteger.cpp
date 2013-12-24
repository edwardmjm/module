/*
	sample input format:
	123 + 345
	123 - 345
	123 + -345
	123 * -345
	1234567890 / -347
*/
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;
#define rep(i,n) for (int i = 0; i < (int)(n); i++)
struct BigInteger {
	bool sgn; //1 for negative
	vector <int> v;
	BigInteger(): sgn(0), v(vector <int> (1, 0)) {}

	BigInteger(const BigInteger &o): sgn(o.sgn), v(o.v) {}

	bool isValidChar(char c) {
		return (c == '-') || ('0' <= c && c <= '9');
	}

	void read() {
		v.clear();
		char c;
		while (!isValidChar(c = getchar()));
		sgn = 0;
		if (c == '-') {
			sgn = 1;
		} else {
			v.push_back(c - '0');
		}
		while ((c = getchar()) && ('0' <= c && c <= '9'))
			v.push_back(c - '0');
		reverse(v.begin(), v.end());
//		rep (i, v.size()) cout << v[i];
//		cout << endl;
		*this = pop();
	}

	void print() {
		if (sgn) cout << "-";
		for (int i = v.size() - 1; i >= 0; i--) cout << v[i];
		cout << endl;
	}

	BigInteger pop() const {
		BigInteger res(*this);
		while (res.v.size() && !res.v.back()) res.v.pop_back();
		if (res.v.empty()) {
			res.v.push_back(0);
			res.sgn = 0;
		}
		return res;
	}

	BigInteger neg() const {
		BigInteger res(*this);
		res.sgn = !res.sgn;
		return res.pop();
	}

	bool operator < (const BigInteger &oth) const {
		BigInteger lhs(*this), rhs(oth);
		if (lhs.sgn && !rhs.sgn) return 1;
		if (!lhs.sgn && rhs.sgn) return 0;
		if (lhs.sgn && rhs.sgn) return rhs.neg() < lhs.neg();
		lhs = lhs.pop();
		rhs = rhs.pop();
		if (lhs.v.size() > rhs.v.size()) return 0;
		if (lhs.v.size() < rhs.v.size()) return 1;
		for (int i = lhs.v.size() - 1; i >= 0; i--) {
			if (lhs.v[i] < rhs.v[i]) return 1;
			if (lhs.v[i] > rhs.v[i]) return 0;
		}
		return 0;
	}

	bool isZero() const {
		rep (i, v.size()) if (v[i]) return 0;
		return 1;
	}

	BigInteger operator + (const BigInteger &oth) const {
		BigInteger res;
		if (sgn)
			if (oth.sgn)
				res.sgn = 1;
			else
				return oth - (*this).neg();
		else
			if (oth.sgn)
				return *this - oth.neg();
		int n = max(oth.v.size(), v.size());
		res.v = vector <int> (n + 1, 0);
		rep (i, n) {
			if (i < (int)v.size()) res.v[i] += v[i];
			if (i < (int)oth.v.size()) res.v[i] += oth.v[i];
		}
		rep (i, n) {
			res.v[i + 1] += res.v[i] / 10;
			res.v[i] %= 10;
		}
		return res.pop();
	}

	BigInteger operator - (const BigInteger &oth) const {
		BigInteger res;
		if (sgn)
			if (oth.sgn)
				return oth.neg() + *this;
			else
				return *this + oth.neg();
		else
			if (oth.sgn)
				return *this + oth.neg();
		if (*this < oth) return (oth - *this).neg();
		int n = v.size();
		res.v = vector <int> (v.size(), 0);
		rep (i, n) {
			res.v[i] = v[i];
			if (i < (int)oth.v.size()) res.v[i] -= oth.v[i];
		}
		rep (i, n - 1) {
			while (res.v[i] < 0) {
				res.v[i + 1]--;
				res.v[i] += 10;
			}
		}
		return res.pop();
	}

	BigInteger operator * (const BigInteger &oth) const {
		BigInteger res;
		res.sgn = sgn ^ oth.sgn;
		res.v = vector <int> (v.size() + oth.v.size() + 5, 0);
		rep (i, v.size()) rep (j, oth.v.size())
			res.v[i + j] += v[i] * oth.v[j];
		rep (i, res.v.size() - 1) {
			res.v[i + 1] += res.v[i] / 10;
			res.v[i] %= 10;
		}
		return res.pop();
	}

	BigInteger operator / (const BigInteger &oth) const {
		if (oth.isZero()) throw "Divide zero";
		BigInteger lhs = (*this);
		BigInteger rhs = (oth);
		BigInteger res;
		bool tmp = lhs.sgn ^ rhs.sgn;
		lhs.sgn = rhs.sgn = 0;
		res.v = vector <int> (v.size(), 0);
		for (int i = lhs.v.size() - 1; i >= 0; i--) {
			for (int j = 0; j < 10; j++) {
				res.v[i] = j;
				if (lhs < res * rhs) {
					res.v[i] = j - 1;
					break;
				}
			}
		}
		res.sgn = tmp;
		return res.pop();
	}
};

int main() {
	BigInteger a, b;
	char op[10];
	a.read();
	scanf("%s", op);
	b.read();
	try {
		if (op[0] == '+') {
			(a + b).print();
		} else if (op[0] == '-') {
			(a - b).print();
		} else if (op[0] == '*') {
			(a * b).print();
		} else if (op[0] == '/') {
			(a / b).print();
		} else {
			puts("Wrong input.");
		}
	} catch (...) {
		cout << "Divide zero" << endl;
	}
}
