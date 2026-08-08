// ============================================================================
// Segment Tree

#include <vector>
#include <utility>
#include <numeric>
#include <optional>

template <class Monoid>
struct SegmentTree {
    SegmentTree(int N, const Monoid& elem = Monoid()) : SegmentTree(std::vector<Monoid>(N, elem)) {}
    
    SegmentTree(const std::vector<Monoid>& a) : N(a.size()), elem(4 * N) {
        build(1, 0, N, a);
    }
    
    // Point modification
    void modify(int id, const Monoid& x) {
        if (id < 0 || id >= N) return;
        operate(1, 0, N, id, id + 1, [&](int o) {
            apply_val(o, x);
        });
    }
    
    // Range query
    Monoid operator () (int L, int R) {
        if (L >= R) return Monoid();
        std::optional<Monoid> res;
        operate(1, 0, N, L, R, [&](int o) {
            if (!res) res = elem[o];
            else *res = *res + elem[o];
        });
        return res ? *res : Monoid();
    }
    
    // Find first index satisfying predicate
    template <class F>
    int findf(int L, int R, F&& pred) {
        if (L >= R) return N;
        return findf(1, 0, N, L, R, pred);
    }
    
    // Find last index satisfying predicate
    template <class F>
    int findl(int L, int R, F&& pred) {
        if (L >= R) return -1;
        return findl(1, 0, N, L, R, pred);
    }

private:
    int N;
    std::vector<Monoid> elem;
    
    #define lc (o << 1)
    #define rc (o << 1 | 1)
    #define m std::midpoint(l, r)
    
    // Apply point assignment
    void apply_val(int o, const Monoid& x) {
        elem[o] = x;    
    }
    
    // Pull values from children up to parent
    void pull(int o) {
        elem[o] = elem[lc] + elem[rc]; 
    }
    
    // Build tree recursively
    void build(int o, int l, int r, const std::vector<Monoid>& a) {
        if (r - l == 1) {
            elem[o] = a[l];
            return;
        }
        build(lc, l, m, a);
        build(rc, m, r, a);
        pull(o);
    }
    
    // Core traversal helper for updates and queries
    template <class Callback>
    void operate(int o, int l, int r, int L, int R, Callback&& cb) {
        if (r <= L || R <= l) return;
        if (L <= l && r <= R) {
            cb(o); 
            return;
        }
        operate(lc, l, m, L, R, std::forward<Callback>(cb));
        operate(rc, m, r, L, R, std::forward<Callback>(cb));
        pull(o);
    }
    
    template <class F>
    int findf(int o, int l, int r, int L, int R, F&& pred) {
        if (l >= R || r <= L) return N;
        if (l >= L && r <= R && !pred(elem[o])) return N;
        if (r - l == 1) return l;
        int res = findf(lc, l, m, L, R, pred);
        if (res == N) {
            res = findf(rc, m, r, L, R, pred);
        }
        return res;
    }
    
    template <class F>
    int findl(int o, int l, int r, int L, int R, F&& pred) {
        if (l >= R || r <= L) return -1;
        if (l >= L && r <= R && !pred(elem[o])) return -1;
        if (r - l == 1) return l;
        int res = findl(rc, m, r, L, R, pred);
        if (res == -1) {
            res = findl(lc, l, m, L, R, pred);
        }
        return res;
    }
    #undef lc
    #undef rc
    #undef m
};

// Data Monoid (Node Value Structure)
struct Monoid {
    // Action of tag on data
    
};

// Pull/Merge operation for two data nodes
Monoid operator + ([[maybe_unused]] const Monoid& l, [[maybe_unused]] const Monoid& r) {
    // --------------------------------
    // write here
    
    return { };
    // --------------------------------
}