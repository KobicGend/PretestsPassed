// ============================================================================
// Binary Indexed Tree (Fenwick Tree)

#include <vector>
#include <cassert>
#include <bit>

template <typename T>
class BinaryIndexedTree {
    int N;
    std::vector<T> f;
    
    static constexpr int lowbit(int x) {
        return x & -x;
    }
    
public:
    BinaryIndexedTree(int N = 0) : N(N), f(N + 1) {}
    
    BinaryIndexedTree(const std::vector<T>& arr) : N(arr.size()), f(arr.size() + 1) {
        for (int i = 1; i <= N; i++) {
            f[i] += arr[i - 1];
            int j = i + lowbit(i);
            if (j <= N) {
                f[j] += f[i];
            }
        }
    }
    
    // Point update O(log N): adds v to the element at 0-based index x
    void add(int x, const T& v) {
        assert(0 <= x && x < N);
        for (int i = x + 1; i <= N; i += lowbit(i)) {
            f[i] += v;
        }
    }
    
    // Prefix sum query O(log N): returns the sum of the range [0, x)
    T operator()(int x) const {
        assert(0 <= x && x <= N);
        T res {};
        for (int i = x; i > 0; i -= lowbit(i)) {
            res += f[i];
        }
        return res;
    }
    
    // Range sum query O(log N): returns the sum of the range [L, R)
    T operator()(int L, int R) const {
        assert(0 <= L && L <= R && R <= N);
        return operator()(R) - operator()(L);
    }

    // Tree binary search (binary lifting) O(log N): finds the minimum index where prefix sum >= target
    // NOTE: ALL elements maintained in the binary indexed tree MUST be non-negative
    int lower_bound(T target) const {
        if (target <= 0) {
            return 0;
        }
        int i = 0;
        for (int bit = std::bit_width(static_cast<unsigned>(N)) - 1; bit != -1; bit--) {
            int j = i + (1 << bit);
            if (j <= N && f[j] < target) {
                target -= f[j];
                i = j;
            }
        }
        return i + 1; // in range [0, N + 1], matching std::lower_bound() - begin() on a 0-padded prefix array
    }
};