#include <bits/stdc++.h>

const int maxn = 2e6 + 100;
const int inf = 1e9;

namespace Tree {
    const int inf = 1e8;
    int ch[maxn << 1][28], link[maxn << 1], lef[maxn << 1], len[maxn << 1], now, rem, n, s[maxn], tl;

    void init() {
        len[0] = inf;
        now = tl = 1;
    }

    int new_node(int st, int l) {
        lef[++tl] = st;
        len[tl] = l;
        return tl;
    }

    void extend(int x) {
        s[++n] = x;
        rem++;
        for (int last = 1; rem;) {
            while (rem > len[ch[now][s[n - rem + 1]]])
                rem -= len[now = ch[now][s[n - rem + 1]]];
            int &v = ch[now][s[n - rem + 1]];
            int c = s[lef[v] + rem - 1];
            if ((!v) || (x == c)) {
                link[last] = now;
                last = now;
                if (!v) v = new_node(n, inf);
                else break;
            } else {
                int u = new_node(lef[v], rem - 1);
                lef[v] += rem - 1;
                len[v] -= rem - 1;
                ch[u][c] = v;
                ch[u][x] = new_node(n, inf);
                link[last] = v = u;
                last = u;
            }
            if (now != 1) now = link[now];
            else rem--;
        }
    }
}

int n, q, ans[maxn], l1[maxn], r1[maxn], l2[maxn], r2[maxn], k[maxn];
std::vector<int> queries[maxn], nodes[maxn];

int d[maxn], l[maxn], r[maxn], anc[maxn][20], len[maxn], ptr[maxn], lmost[maxn], rmost[maxn];

void dfs(int u, int fa, int dep = 0) {
    dep += std::min(Tree::len[u], n - Tree::lef[u] + 1);
    len[u] = dep;
    anc[u][0] = fa;
    for (int i = 1; i <= 19; ++i)
        anc[u][i] = anc[anc[u][i - 1]][i - 1];
    int is_leaf = 1;
    lmost[u] = inf;
    rmost[u] = -inf;
    for (int i = 0; i <= 26; ++i) {
        int v = Tree::ch[u][i];
        if (!v) continue;
        is_leaf = 0;
        d[v] = d[u] + 1;
        dfs(v, u, dep);
        lmost[u] = std::min(lmost[u], lmost[v]);
        rmost[u] = std::max(rmost[u], rmost[v]);
    }
    if (is_leaf) {
        l[u] = r[u] = n - dep + 1;
        ptr[n - dep + 1] = u;
        lmost[u] = rmost[u] = n - dep + 1;
    } else {
        nodes[dep].push_back(u);
    }
}

int lca(int u, int v) {
    if (d[u] < d[v]) std::swap(u, v);
    for (int i = 19; i >= 0; --i)
        if (d[u] - (1 << i) >= d[v])
            u = anc[u][i];
    if (u == v) return u;
    for (int i = 19; i >= 0; --i)
        if (anc[u][i] != anc[v][i])
            u = anc[u][i], v = anc[v][i];
    return anc[u][0];
}

int locate(int lp, int rp) {
    int u = ptr[lp];
    for (int i = 19; i >= 0; i--)
        if (len[anc[u][i]] >= (rp - lp + 1))u = anc[u][i];
    return u;
}

int lcp(int x, int y) {
    return len[lca(ptr[x], ptr[y])];
}

int is_equal(int la, int ra, int lb, int rb) {
    return (ra - la == rb - lb) and (lcp(la, lb) >= ra - la + 1);
}

namespace LCT {
    int fa[maxn], ch[maxn][2], sum[maxn], w[maxn], min[maxn], tl, id[maxn];

    int not_root(int x) { return (ch[fa[x]][1] == x) || (ch[fa[x]][0] == x); }

    void update(int x) {
        sum[x] = sum[ch[x][0]] + sum[ch[x][1]] + w[x];
        min[x] = std::min({min[ch[x][0]] ? min[ch[x][0]] : inf, min[ch[x][1]] ? min[ch[x][1]] : inf, id[x] ? id[x] : inf});
    }

    void rotate(int x) {
        int f = fa[x], g = fa[f], l = ch[fa[x]][1] == x, c = ch[x][l ^ 1];
        if (not_root(f)) ch[g][ch[g][1] == f] = x;
        ch[x][l ^ 1] = f;
        ch[f][l] = c;
        fa[x] = g;
        fa[f] = x;
        if (c) fa[c] = f;
        update(f);
        update(x);
    }

    void splay(int x) {
        while (not_root(x)) {
            if (not_root(fa[x])) rotate((ch[fa[x]][1] == x) == (ch[fa[fa[x]]][1] == fa[x]) ? fa[x] : x);
            rotate(x);
        }
    }

    void access(int x) {
        for (int y = 0; x; x = fa[y = x]) {
            splay(x);
            ch[x][1] = y;
            update(x);
        }
    }

    void link(int x, int y) { // x -> y
        access(x);
        splay(x);
        fa[x] = y;
    }

    void cut(int x) {
        access(x);
        splay(x);
        fa[ch[x][0]] = 0;
        ch[x][0] = 0;
        update(x);
    }

    int new_node(int weight, int ind) {
        int u = ++tl;
        sum[u] = w[u] = weight;
        min[u] = id[u] = ind;
        return u;
    }

    int find(int u, int p) {
        access(u);
        splay(u);
        int tot = 3;
        while (u) {
            if (min[u] > p) return -1;
            if (ch[u][1] and min[ch[u][1]] <= p) u = ch[u][1];
            else if (id[u] and id[u] <= p) {
                return sum[ch[u][1]] + tot;
            } else {
                tot += sum[ch[u][1]] + w[u];
                u = ch[u][0];
            }
        }
        return -1;
    }
}

int main() {
    std::ios::sync_with_stdio(false);
    std::string s;
    std::cin >> s >> q;
    Tree::init();
    for (char c: s)
        Tree::extend(c - 'a');
    Tree::extend(26);
    n = (int) s.length();
    s = ' ' + s;
    dfs(1, 0);
    for (int i = 1; i <= q; ++i) {
        std::cin >> l1[i] >> r1[i] >> l2[i] >> r2[i] >> k[i];
        assert(r1[i] - l1[i] + 1 >= k[i]);
        assert(r2[i] - l2[i] + 1 >= k[i]);
        if (is_equal(l1[i], r1[i], l2[i], r2[i])) ans[i] = 1;
        else if (lcp(r1[i] - k[i] + 1, l2[i]) >= k[i]) ans[i] = 2;
        else queries[k[i]].push_back(i);
    }
    std::set<std::pair<std::pair<int, int>, std::pair<int, int> > > st;
    for (int i = 1; i <= n; ++i) {
        st.insert({{i, i},
                   {i, LCT::new_node(1, i)}});
    }
    for (int i = n; i >= 1; i--) {
        for (int u: nodes[i]) {
            l[u] = n;
            r[u] = 1;
            for (int j = 0; j <= 26; ++j) {
                int v = Tree::ch[u][j];
                if (!v) continue;
                l[u] = std::min(l[v], l[u]);
                r[u] = std::max(r[v], r[u]);
            }
            auto it = st.upper_bound({{r[u], r[u]},
                                      {inf,  inf}});
            assert(it != st.begin());
            it = std::prev(it);
            std::vector<std::pair<std::pair<int, int>, std::pair<int, int> >>bin, inc;
            int y = LCT::new_node(0, 0);
            int tL = inf, tR = -inf;
            while (true) {
                auto [p1, p2] = *it;
                auto [L, R] = p1;
                auto [f, x] = p2;
                if (f <= l[u]) break;
                assert(L >= l[u]);
                LCT::cut(x);
                LCT::link(x, y);
                bin.push_back(*it);
                tL = std::min(tL, L);
                tR = std::max(tR, R);
                if (it != st.begin()) it = std::prev(it);
                else break;
            }
            for (auto x : bin) st.erase(x);
            if (tL <= tR) {
                LCT::link(y, l[u]);
                st.insert({{tL,   tR},
                           {l[u], y}});
            }
        }
        for (auto x: queries[i]) {
            int p1 = lmost[locate(r1[x] - k[x] + 1, r1[x])];
            int p2 = lmost[locate(l2[x], l2[x] + k[x] - 1)];
            ans[x] = LCT::find(p1, p2);
        }
    }
    for (int i = 1; i <= q; ++i)
        std::cout << ans[i] << '\n';
    return 0;
}