# [2026 ICPC 深圳邀请赛 H. Telepathy](https://qoj.ac/contest/3588/problem/17760?v=1)

## Tag

[Communication](Communication.md)

## Description

- 给定 01 串 $S$，满足其中 1 的数量严格小于 0 的数量。
- First 必须将 $S$ 的某个 0 改为 1，得到 $S'$。
- Second 依据 $S'$ 还原 $S$。
- $|S| \leqslant 10^{6}$。

## Detailed Solution

为思考方便，可以把题目出现的数字变成 1，没出现的变成 0，这样得到一个完整的 01 串。设 $N = |S|$，$K$ 为 1 的数量。

### Hint1

*先考虑 **最坏的情况**。*

最坏的情况是 $N$ 为奇数且 $K=\dfrac{N-1}{2}$，初始 01 串有 $\dbinom{N}{K}$ 种，修改后有 $\dbinom{N}{K+1}$ 种，这两个组合数相等。

### Hint2

*在这种情况下，须构造一个 **双射**。*

要双射，所以尽可能把结构相同的东西都映射到另一种结构相同的东西上。

### Hint3

什么是结构相同？比如 $N=5$，`1 2` 和 `2 3`，甚至和 `5 1` 都应当算作结构相同，即两个出现的数挨着。

*考虑 **cyclic shift**（循环移位）。*

### Hint4

这时你可能会想到，字典序最小/大的循环移位（或若干变种），尝试后发现不行。最根本的原因是「字典序」是一个局部特征，如果第一位不一样，那后面若干位都不需要比较——这就失去了很多信息。

也就是说需要找到，只有某一种循环移位满足某条件，而且这个条件是全局性，涉及数组每一项的。

*现在尝试 **把 1 换成 ╱，0 换成 ╲**，把他们连接起来。再试试循环移位？*

### Solution

你发现了一个惊人的结论：

- 和为 1 的整数数组，只有恰好一种循环移位方式，满足除去第 0 项的所有前缀和严格正；等价地，和为 -1 的数组，只有一种满足除去整体的所有前缀和非负。

你知道这一定就是正解。现在只需要考虑怎么实现了。

- First Run 是一个和为 -1 的数组，找到第一次出现的前缀和最小值的下标，将这个数变成 1。（因为满足所有前缀和非负的循环移位数组，这个下标恰好在最后一个位置，保证唯一性。）
- Second Run 是一个和为 1 的数组，找到最后一次出现的前缀和最小值的下标，这就是刚才变成 1 的位置。（因为满足所有前缀和严格正的循环移位数组，这个下标恰好在第一个位置，保证唯一性。）

对于原题，-1 的个数远多于 1，采用如上方案仍然正确。

## Code

```cpp
#include <bits/stdc++.h>
using i64 = long long;

int main() {
    std::string opt;
    std::cin >> opt;

    int T;
    std::cin >> T;
    
    while (T--) {
        int N, K;
        std::cin >> N >> K;
        if (opt == "Bob") {
            K++;
        }

        std::vector<int> A(N, -1);
        while (K--) {
            int x;
            std::cin >> x;
            x--;
            A[x] = 1;
        }

        std::vector<int> pre(N + 1);
        for (int i = 0; i < N; i++) {
            pre[i + 1] = pre[i] + A[i];
        }
        int imin = -1;
        if (opt == "Alice") {
            for (int i = 1; i <= N; i++) {
                if (imin == -1 || pre[i] < pre[imin]) {
                    imin = i;
                }
            }
            imin--;
            A[imin] = 1;
            for (int i = 0; i < N; i++) {
                if (A[i] == 1) {
                    std::cout << i + 1 << " ";
                }
            }
            std::cout << "\n";
        } else {
            for (int i = 0; i < N; i++) {
                if (imin == -1 || pre[i] <= pre[imin]) {
                    imin = i;
                }
            }
            imin++;
            std::cout << imin << "\n";
        }
    }

    return 0;
}
```

