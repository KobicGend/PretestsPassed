// ============================================================================
// Implicit Treap

#include <vector>
#include <utility>
#include <optional>
#include <random>
#include <algorithm>

template <class Monoid, class End>
struct Treap {
    Treap(int N, const Monoid& elem = Monoid()) : Treap(std::vector<Monoid>(N, elem)) {}
    
    Treap(const std::vector<Monoid>& a) {
        tree.reserve(a.size() + 1);
        tree.push_back({}); // Node 0 as null dummy
        tree[0].size = 0;
        
        // Cartesian tree O(N) construction
        std::vector<int> stk;
        stk.reserve(a.size());
        for (const auto& val : a) {
            int u = new_node(val);
            int last = 0;
            while (!stk.empty() && tree[stk.back()].priority < tree[u].priority) {
                last = stk.back();
                stk.pop_back();
            }
            if (last) tree[u].lc = last;
            if (!stk.empty()) tree[stk.back()].rc = u;
            stk.push_back(u);
        }
        if (!stk.empty()) root = stk.front();
        
        auto build_pull = [&](auto self, int o) -> void {
            if (!o) return;
            self(self, tree[o].lc);
            self(self, tree[o].rc);
            pull(o);
        };
        build_pull(build_pull, root);
    }
    
    int size() const { return tree[root].size; }
    
    // Insert single element at pos (0-indexed)
    void insert(int pos, const Monoid& x) {
        if (pos < 0 || pos > size()) return;
        int A = 0, B = 0;
        split(root, pos, A, B);
        int u = new_node(x);
        root = merge(merge(A, u), B);
    }
    
    // Erase single element at pos (0-indexed)
    void erase(int pos) {
        if (pos < 0 || pos >= size()) return;
        erase(pos, pos + 1);
    }
    
    // Erase range [L, R)
    void erase(int L, int R) {
        if (L >= R || L < 0 || R > size()) return;
        int A = 0, B = 0, C = 0;
        split(root, R, A, C);
        split(A, L, A, B);
        recycle(B);
        root = merge(A, C);
    }
    
    // Point modification
    void modify(int id, const Monoid& x) {
        if (id < 0 || id >= size()) return;
        operate(id, id + 1, [&](int o) {
            tree[o].val = x;
            tree[o].tag = std::nullopt;
            pull(o);
        });
    }
    
    // Range update
    void update(int L, int R, const End& x) {
        if (L >= R) return;
        operate(L, R, [&](int o) {
            apply_tag(o, x);
        });
    }
    
    // Range reversal (Literary Treap Feature)
    void reverse(int L, int R) {
        if (L >= R) return;
        operate(L, R, [&](int o) {
            apply_rev(o);
        });
    }
    
    // Range cyclic shift right by k steps
    void shift(int L, int R, int k) {
        if (L >= R) return;
        int len = R - L;
        k = (k % len + len) % len;
        if (k == 0) return;
        
        int A = 0, B = 0, C = 0;
        split(root, R, A, C);
        split(A, L, A, B);
        
        int B1 = 0, B2 = 0;
        split(B, len - k, B1, B2);
        B = merge(B2, B1);
        
        root = merge(merge(A, B), C);
    }
    
    // Range query
    Monoid operator () (int L, int R) {
        if (L >= R) return Monoid();
        Monoid res;
        operate(L, R, [&](int o) {
            res = tree[o].sum;
        });
        return res;
    }

private:
    struct Node {
        Monoid val;
        Monoid sum;
        std::optional<End> tag;
        bool rev = false;
        int size = 1;
        uint32_t priority = 0;
        int lc = 0, rc = 0;
    };

    int root = 0;
    std::vector<Node> tree;
    std::vector<int> free_list;
    std::mt19937 rng{std::random_device{}()};

    int new_node(const Monoid& x) {
        int id;
        if (!free_list.empty()) {
            id = free_list.back();
            free_list.pop_back();
            tree[id] = Node{x, x, std::nullopt, false, 1, rng(), 0, 0};
        } else {
            id = tree.size();
            tree.push_back(Node{x, x, std::nullopt, false, 1, rng(), 0, 0});
        }
        return id;
    }

    void recycle(int o) {
        if (!o) return;
        recycle(tree[o].lc);
        recycle(tree[o].rc);
        free_list.push_back(o);
    }

    // Apply lazy tag to data and compose with existing tag
    void apply_tag(int o, const End& x) {
        if (!o) return;
        tree[o].val *= x;
        tree[o].sum *= x;
        if (!tree[o].tag) tree[o].tag = x;
        else *tree[o].tag *= x;
    }

    // Apply lazy topology reverse flag
    void apply_rev(int o) {
        if (!o) return;
        tree[o].rev = !tree[o].rev;
        tree[o].val.reverse();
        tree[o].sum.reverse();
        std::swap(tree[o].lc, tree[o].rc);
    }

    // Push lazy tags down to children
    void push(int o) {
        if (!o) return;
        if (tree[o].rev) {
            apply_rev(tree[o].lc);
            apply_rev(tree[o].rc);
            tree[o].rev = false;
        }
        if (tree[o].tag) {
            apply_tag(tree[o].lc, *tree[o].tag);
            apply_tag(tree[o].rc, *tree[o].tag);
            tree[o].tag = std::nullopt;
        }
    }

    // Pull values from children up to parent
    void pull(int o) {
        if (!o) return;
        tree[o].size = tree[tree[o].lc].size + tree[tree[o].rc].size + 1;
        
        std::optional<Monoid> res;
        if (int lc = tree[o].lc) res = tree[lc].sum;
        
        if (!res) res = tree[o].val;
        else *res = *res + tree[o].val;
        
        if (int rc = tree[o].rc) *res = *res + tree[rc].sum;
        
        tree[o].sum = res ? *res : Monoid();
    }

    // Split tree 'o' by size k into subtrees 'x' [0, k) and 'y' [k, size)
    void split(int o, int k, int& x, int& y) {
        if (!o) { x = y = 0; return; }
        push(o);
        int left_size = tree[tree[o].lc].size;
        if (k <= left_size) {
            y = o;
            split(tree[o].lc, k, x, tree[o].lc);
            pull(y);
        } else {
            x = o;
            split(tree[o].rc, k - left_size - 1, tree[o].rc, y);
            pull(x);
        }
    }

    // Merge subtrees 'x' and 'y' into one tree
    int merge(int x, int y) {
        if (!x || !y) return x ? x : y;
        if (tree[x].priority > tree[y].priority) {
            push(x);
            tree[x].rc = merge(tree[x].rc, y);
            pull(x);
            return x;
        } else {
            push(y);
            tree[y].lc = merge(x, tree[y].lc);
            pull(y);
            return y;
        }
    }

    // Core range-extraction traversal helper using Split/Merge
    template <class Callback>
    void operate(int L, int R, Callback&& cb) {
        if (L >= R || L < 0 || R > size()) return;
        int A = 0, B = 0, C = 0;
        split(root, R, A, C);
        split(A, L, A, B);
        
        cb(B); // B is the subtree representing range [L, R)
        
        root = merge(merge(A, B), C);
    }
};

// Endomorphism Monoid (Lazy Tag Structure)
struct End {
    // Composition of two tags
    End& operator *= ([[maybe_unused]] const End& f) {
        // --------------------------------
        // write here
        
        // --------------------------------
        return *this;
    }
};

// Data Monoid (Node Value Structure)
struct Monoid {
    // Action of tag on data
    Monoid& operator *= ([[maybe_unused]] const End& f) {
        // --------------------------------
        // write here
        
        // --------------------------------
        return *this;
    }

    // Optional: Only needed if operation is non-commutative (e.g. matrix mul)
    void reverse() {
        // --------------------------------
        // write here if needed
        
        // --------------------------------
    }
};

// Pull/Merge operation for data nodes
Monoid operator + ([[maybe_unused]] const Monoid& l, [[maybe_unused]] const Monoid& r) {
    // --------------------------------
    // write here
    
    return { };
    // --------------------------------
}