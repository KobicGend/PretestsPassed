// ============================================================================
// Sparse Table (Range Minimum/Maximum Query)

#include <vector>
#include <algorithm>
#include <numeric>
#include <cassert>
#include <utility>
#include <bit>      // for std::bit_width

template <typename T, typename Op>
class SparseTable {
    std::vector<std::vector<T>> f;
    Op op;

public:
    // Build O(N log N)
    explicit SparseTable(std::vector<T> arr, Op _op) : op(std::move(_op)) {
        const unsigned N = arr.size();
        if (N == 0) return;
        f.resize(std::bit_width(N));
        f[0] = std::move(arr);
        for (unsigned bit = 0; bit + 1 < f.size(); bit++) {
            f[bit + 1].resize(N + 1 - (2U << bit));
            for (unsigned i = 0; i + (2U << bit) <= N; i++) {
                f[bit + 1][i] = op(f[bit][i], f[bit][i + (1U << bit)]);
            }
        }
    }

    // Range query O(1), [L, R)
    T operator()(unsigned L, unsigned R) const {
        assert(L < R && R <= f[0].size());
        unsigned bit = std::bit_width(R - L) - 1;
        return op(f[bit][L], f[bit][R - (1U << bit)]);
    }
};

template <typename T, typename Op>
auto IndexSparseTable(const std::vector<T>& arr, Op op) {
    std::vector<unsigned> id(arr.size());
    std::ranges::iota(id, 0);
    return SparseTable(id, std::move(op));
}

// auto gcd = SparseTable(A, [](auto x, auto y) { return std::gcd(x, y); });
// auto min = IndexSparseTable(A, [&](auto i, auto j) { return A[i] <= A[j] ? i : j; });