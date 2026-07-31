// ============================================================================
// Euler Sieve

#include <vector>

std::vector<int> spf;   // the smallest prime factor of n, spf[n] = n iff n is prime
std::vector<int> primes; // list of primes

static const bool sieve_init = [] {
    constexpr int maxN = 10'000'000;  // change here if needed
    spf.resize(maxN + 1);
    spf[1] = 1;
    for (int n = 2; n < std::ssize(spf); n++) {
        if (!spf[n]) {
            spf[n] = n;
            primes.push_back(n);
        }
        for (int p : primes) {
            if (1LL * n * p >= std::ssize(spf)) {
                break;
            }
            spf[n * p] = p;
            if (p == spf[n]) {
                break;
            }
        }
    }
    return 0;
} ();