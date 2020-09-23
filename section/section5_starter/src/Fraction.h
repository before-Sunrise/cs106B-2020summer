#pragma once

#include<ostream>
using namespace std;

class Fraction {
public:
    Fraction();
    Fraction(int numer,int denom);

    void add(const Fraction &f);
    void mult(const Fraction &f);
    double decimal();
    int getNumer();
    int getDenom();
    void reciprocal();
    void divide(const Fraction &f);
    friend ostream& operator<<(ostream& out, const Fraction &frac);
    friend Fraction operator*(const Fraction &first, const Fraction &second);
    friend bool operator<(const Fraction &first, const Fraction &second);
private:
    int numer;   // the numerator
    int denom; // the denominator
    void reduce(); // reduce the fraction to lowest terms
    int gcd(int u, int v);
};
