// ============================================================================
// Segment Set

#include <iterator>
#include <set>
#include <utility>

template <class T, class V>
struct SegmentSet {
    struct Segment {
        T l;
        mutable T r;
        mutable V value;
        
        bool operator < (const Segment& o) const {
            return l < o.l;
        }
        bool operator < (const T& val) const {
            return l < val;
        }
        friend bool operator < (const T& val, const Segment& o) {
            return val < o.l;
        }
    };

    std::set<Segment, std::less<>> s;

    SegmentSet() = default;
    SegmentSet(const T& L, const T& R, V value) {
        assign(L, R, std::move(value));
    }

    // try merging current segment with the next one
    bool try_merge_next(auto it) {
        auto next = std::next(it);
        if (next != s.end() && it->r == next->l && it->value == next->value) {
            it->r = next->r;
            s.erase(next);
            return true;
        }
        return false;
    }

    // split target segment at x
    // return iterator to segment starting at x
    auto split(const T& x) {
        auto it = s.lower_bound(x);
        if (it == s.begin()) return it;
        if (it != s.end() && it->l == x) return it;
        auto pre = std::prev(it);
        if (x >= pre->r) return it;
        auto y = pre->r;
        pre->r = x;
        return s.insert({x, y, pre->value}).first;
    }

    // merge adjacent segments with identical value
    auto merge(auto it) {
        if (it == s.end()) return it;
        try_merge_next(it);
        if (it == s.begin()) return it;
        auto pre = std::prev(it);
        if (try_merge_next(pre)) return pre;
        return it;
    }

    // find segment containing x
    auto find(const T& x) {
        auto it = s.upper_bound(x);
        if (it == s.begin()) return s.end();
        it = std::prev(it);
        return (x < it->r) ? it : s.end();
    }

    // erase range [L, R)
    // return iterator to element after erased range
    auto erase(const T& L, const T& R) {
        if (L >= R) return split(L);
        auto r = split(R);
        auto l = split(L);
        return s.erase(l, r);
    }

    // assign value to range [L, R)
    auto assign(const T& L, const T& R, V value) {
        if (L >= R) return split(L);
        erase(L, R);
        auto it = s.insert({L, R, value}).first; 
        return merge(it);
    }

    // modify values in range [L, R)
    template <class Cb>
    void modify(const T& L, const T& R, Cb cb) {
        if (L >= R) return;
        auto r = split(R);
        auto l = split(L);
        if (l == r) return;
        for (auto it = l; it != r; it++) {
            cb(it->value);
        }
        auto x = std::prev(r);
        if (l == x) {
            merge(l);
        } else {
            merge(x);
            merge(l);
        }
    }

    // query values in range [L, R)
    template <class Cb>
    void query(const T& L, const T& R, Cb cb) {
        if (L >= R) return;
        auto r = split(R);
        auto l = split(L);
        for (auto it = l; it != r; ++it) {
            cb(it->value, it->l, it->r);
        }
    }
};