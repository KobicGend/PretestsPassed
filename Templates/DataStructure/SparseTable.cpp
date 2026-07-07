// ============================================================================
// Sparse Table (Range Minimum/Maximum Query)

#include <vector>
#include <cassert>
#include <utility>
#include <ranges>
#include <bit>      // for std::bit_width
#include <memory>   // for std::shared_ptr

// ----------------------------------------------------------------------------
// Standard Sparse Table for querying values directly
template <typename T, typename Op>
class SparseTable {
    std::vector<std::vector<T>> st;
    Op op;

public:
    SparseTable() = default;

    // Build O(N log N)
    explicit SparseTable(std::vector<T> _arr, Op _op) : op(std::move(_op)) {
        unsigned N = _arr.size();

        st.resize(std::bit_width(N));
        st[0] = std::move(_arr);

        for (unsigned bit = 0; bit + 1 < st.size(); bit++) {
            st[bit + 1].resize(N + 1 - (2U << bit)); 
            for (unsigned i = 0; i + (2U << bit) <= N; i++) {
                st[bit + 1][i] = op(st[bit][i], st[bit][i + (1U << bit)]);
            }
        }
    }

    // Range query O(1), [L, R)
    T operator()(unsigned L, unsigned R) const {
        assert(L < R && R <= st[0].size());
        unsigned len = R - L;
        unsigned bit = std::bit_width(len) - 1;        
        return op(st[bit][L], st[bit][R - (1U << bit)]);
    }
};

// ----------------------------------------------------------------------------
// Index-based Sparse Table for querying the index of the extremum
template <typename T, typename Op>
class IndexSparseTable {
    std::shared_ptr<std::vector<T>> arr;

    struct IndexOp {
        std::shared_ptr<std::vector<T>> arr;
        Op op;
        
        unsigned operator()(unsigned i, unsigned j) const {
            return op((*arr)[i], (*arr)[j]) == (*arr)[i] ? i : j;
        }
    };    
    SparseTable<unsigned, IndexOp> st;
    
public:
    IndexSparseTable() = default;

    // Build O(N log N)
    explicit IndexSparseTable(std::vector<T> _arr, Op _op) {
        arr = std::make_shared<std::vector<T>>(std::move(_arr));
        auto indices = std::views::iota(0U, arr->size()) | std::ranges::to<std::vector<unsigned>>();
        auto op = IndexOp(arr, std::move(_op));
        st = SparseTable(std::move(indices), std::move(op));
    }

    // Range index query O(1), [L, R)
    auto operator()(unsigned L, unsigned R) const {
        auto idx = st(L, R);
        return std::pair(idx, (*arr)[idx]);
    }
};