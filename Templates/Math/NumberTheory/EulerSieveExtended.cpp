// ============================================================================
// Euler Sieve (Extended)

#include <vector>

std::vector<int> spf;    // smallest prime factor of n, spf[n] = n iff n is prime
std::vector<int> primes; // list of primes

std::vector<int> omega;  // number of prime factors (distinct)
std::vector<int> Omega;  // number of prime factors (with multiplicity)
std::vector<int> pi;     // pi[n] = number of primes <= n
std::vector<int> powspf; // largest power of spf[n] that divides n
std::vector<int> tau;    // number of divisors
std::vector<int> phi;    // Euler's phi function
std::vector<int> mu;     // Möbius function

static const bool sieve_init = [] {
    constexpr int maxN = 10'000'000;  // change here if needed

    spf.resize(maxN + 1);
    omega.resize(maxN + 1);
    Omega.resize(maxN + 1);
    pi.resize(maxN + 1);
    powspf.resize(maxN + 1);
    tau.resize(maxN + 1);
    phi.resize(maxN + 1);
    mu.resize(maxN + 1);

    spf[1] = 1;
    powspf[1] = 1;
    tau[1] = 1;
    phi[1] = 1;
    mu[1] = 1;
    omega[1] = 0;
    Omega[1] = 0;
    pi[1] = 0;

    for (int n = 2; n < std::ssize(spf); n++) {
        if (!spf[n]) {
            spf[n] = n;
            powspf[n] = n;
            primes.push_back(n);
            omega[n] = 1;
            Omega[n] = 1;
            tau[n] = 2;
            phi[n] = n - 1;
            mu[n] = -1;
        }
        pi[n] = std::ssize(primes);

        for (int p : primes) {
            if (1LL * n * p >= std::ssize(spf)) {
                break;
            }
            spf[n * p] = p;
            Omega[n * p] = Omega[n] + 1;

            if (p == spf[n]) {
                powspf[n * p] = powspf[n] * p;
                omega[n * p] = omega[n];
                phi[n * p] = phi[n] * p;
                mu[n * p] = 0;

                if (n * p == powspf[n * p]) {
                    tau[n * p] = tau[n] + 1;
                } else {
                    tau[n * p] = tau[powspf[n * p]] * tau[n / powspf[n]];
                }

                break;
            }

            powspf[n * p] = p;
            omega[n * p] = omega[n] + 1;
            phi[n * p] = phi[n] * (p - 1);
            mu[n * p] = -mu[n];
            tau[n * p] = tau[n] * 2;
        }
    }
    return 0;
} ();