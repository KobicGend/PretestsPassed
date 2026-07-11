// ============================================================================
// Euler Sieve

#include <vector>

std::vector<int> minp;   // the smallest prime factor of n, minp[n] = n iff n is prime
std::vector<int> primes; // list of primes

static const bool sieve_init = [] {
    constexpr int maxN = 10'000'000;  // change here if needed
    minp.resize(maxN + 1);
    minp[1] = 1;
    for (int n = 2; n < std::ssize(minp); n++) {
        if (!minp[n]) {
            minp[n] = n;
            primes.push_back(n);
        }
        for (int p : primes) {
            if (1LL * n * p >= std::ssize(minp)) {
                break;
            }
            minp[n * p] = p;
            if (p == minp[n]) {
                break;
            }
        }
    }
    return 0;
} ();