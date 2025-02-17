#include <bits/stdc++.h>


int main() {
    int n, q;
    std::string s;
    std::cin >> s >> q;
    n = (int) s.length();
    s = ' ' + s;
    std::vector<std::string> substrings;
    for (int i = 1; i <= n; ++i)
        for (int len = 1; len <= n - i + 1; ++len)
            substrings.push_back(s.substr(i, len));
    std::sort(substrings.begin(), substrings.end());
    substrings.erase(std::unique(substrings.begin(), substrings.end()), substrings.end());
    auto solve = [&](int l1, int r1, int l2, int r2, int k) {
        std::map<std::string, int> dis;
        dis[s.substr(l1, r1 - l1 + 1)] = 1;
        std::queue<std::string> q;
        q.push(s.substr(l1, r1 - l1 + 1));
        while (not q.empty()) {
            auto u = q.front();
            q.pop();
            for (const auto &v: substrings) {
                if (u.substr(u.length() - k, k) == v.substr(0, k) and not dis[v]) {
                    dis[v] = dis[u] + 1;
                    q.push(v);
                }
            }
        }
        int result = dis[s.substr(l2, r2 - l2 + 1)];
        return result ? result : -1;
    };
    while (q--) {
        int l1, r1, l2, r2, k;
        std::cin >> l1 >> r1 >> l2 >> r2 >> k;
        std::cout << solve(l1, r1, l2, r2, k) << '\n';
    }
    return 0;
}