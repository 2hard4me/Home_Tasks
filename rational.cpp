#include <iostream>
#include <sstream>
#include <string>


using namespace std;


int GCD(int a, int b) {
	return (b == 0) ? a : GCD(b, a % b);
}


class Rational {
public:
	Rational() {
		numer = 0;
		denom = 1;
  	}
	Rational(int tmp_numer, int tmp_denom) {
		int gcd = GCD(tmp_numer, tmp_denom);
		numer = tmp_numer / gcd;
		denom = tmp_denom / gcd;
		if (denom < 0) {
			denom = -denom;
			numer = -numer;
		}
	}
 
	int Numerator() const {
		return numer;
	}
 
	int Denominator() const {
		return denom;
	}
 
private:
	int numer;
	int denom;
};


bool operator == (const Rational& first, const Rational& second) {
	return first.Numerator() == second.Numerator() &&
		first.Denominator() == second.Denominator();
}


Rational operator + (const Rational& first, const Rational& second) {
	return {
		first.Numerator() * second.Denominator() + second.Numerator() * first.Denominator(),
		first.Denominator() * second.Denominator()
	};
}
 

Rational operator - (const Rational& first, const Rational& second) {
	return {
		first.Numerator() * second.Denominator() - second.Numerator() * first.Denominator(),
		first.Denominator() * second.Denominator()
	};
}


Rational operator * (const Rational& first, const Rational& second) {
	return {
		first.Numerator() * second.Numerator(),
		first.Denominator() * second.Denominator()
	};
}
 

Rational operator / (const Rational& first, const Rational& second) {
	return first * Rational(second.Denominator(), second.Numerator());
}
 
istream& operator >> (istream& stream, Rational& r) {
	int numer, denom = 0;
	if (stream) {
		stream >> numer;
		stream.ignore(1);
		stream >> denom;
		if (!denom) {
			r = Rational(numer, denom);
		}
	}
 
	return stream;
}
 
ostream& operator << (ostream& stream, const Rational& r) {
	return stream << r.Numerator() << '/' << r.Denominator();
}
 

bool operator < (const Rational& first, const Rational& second) {
	return (first - second).Numerator() < 0;
}

