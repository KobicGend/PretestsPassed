// ============================================================================
// Fraction

#include <numeric>
#include <iostream>
#include <compare>
#include <cassert>

template<class T>
struct Frac {
    T num; // numerator
    T den; // denominator

    constexpr Frac(T num_ = 0, T den_ = 1) : num(num_), den(den_) {
        assert(den != 0);
        if (den < 0) {
            den = -den;
            num = -num;
        }
        T g = std::gcd(num, den);
        num /= g;
        den /= g;
    }

    constexpr explicit operator double() const {
        return static_cast<double>(num) / den;
    }
    constexpr explicit operator long double() const {
        return static_cast<long double>(num) / den;
    }

    constexpr Frac operator - () const {
        return Frac(-num, den);
    }
    
    constexpr Frac& operator += (const Frac& b)& {
        return *this = Frac(num * b.den + b.num * den, den * b.den);
    }
    constexpr Frac& operator -= (const Frac& b)& {
        return *this = Frac(num * b.den - b.num * den, den * b.den);
    }
    constexpr Frac& operator *= (const Frac& b)& {
        return *this = Frac(num * b.num, den * b.den);
    }
    constexpr Frac& operator /= (const Frac& b)& {
        return *this = Frac(num * b.den, den * b.num);
    }
    
    friend constexpr Frac operator + (Frac a, const Frac& b) { return a += b; }
    friend constexpr Frac operator - (Frac a, const Frac& b) { return a -= b; }
    friend constexpr Frac operator * (Frac a, const Frac& b) { return a *= b; }
    friend constexpr Frac operator / (Frac a, const Frac& b) { return a /= b; }
    
    bool operator == (const Frac& b) const = default;

    friend constexpr std::strong_ordering operator <=> (const Frac& a, const Frac& b) {
        return a.num * b.den <=> b.num * a.den;
    }

    friend std::ostream& operator << (std::ostream &os, const Frac& x) {
        if (x.den == 1) {
            return os << x.num;
        } else {
            return os << x.num << "/" << x.den;
        }
    }
};