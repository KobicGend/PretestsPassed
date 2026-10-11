// ============================================================================
// Cartesian Tree

#include <vector>
#include <stack>

void solve(std::vector<int>& A) {
    const int N = int(A.size());
    std::vector ch(N, std::array{ -1, -1 });
    std::stack<int> stk;
    for (int x = 0; x < N; x++) {
        while (!stk.empty() && A[x] < A[stk.top()]) {
            // the root is the smallest element
            int y = stk.top();
            ch[y][1] = ch[x][0];
            ch[x][0] = y;
            stk.pop();
        }
        stk.push(x);
    }
    while (stk.size() > 1) {
        int x = stk.top();
        stk.pop();
        ch[stk.top()][1] = x;
    }
    int rt = stk.top();

    auto dfs = [&](this auto&& self, int x, int l, int r) -> void {
        if (x == -1) {
            return;
        }
        self(ch[x][0], l, x);
        self(ch[x][1], x + 1, r);
    };
    dfs(rt, 0, N);
}