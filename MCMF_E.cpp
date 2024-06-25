#include <bits/stdc++.h>
#define int long long
using namespace std;

struct edge {
    int u, v;
    int f{0};
    int c;
    int w;

    edge(int u, int v, int f, int c, int w) : u(u), v(v), f(f), c(c), w(w) {}
};

struct MCMF {
    const int INF = 1e18;
    int n, s, f;
    vector<edge> edges;
    vector<vector<int>> graph;
    vector<int> dist;
    vector<int> par;
    vector<int> ptr;

    void add_edge(int u, int v, int cap, int weight) {
        graph[u].push_back(edges.size());
        graph[v].push_back(edges.size() + 1);
        edges.emplace_back(u, v, 0, cap, weight);
        edges.emplace_back(v, u, 0, 0, -weight);
    }

    explicit MCMF(int n, int s, int f) : n(n), s(s), f(f) {
        graph.resize(n);
        dist.assign(n, -1);
        ptr.resize(n);
    }

    int res(int e) {
        return edges[e].c - edges[e].f;
    }

    bool spfa() {
        dist.assign(n, INF);
        par.assign(n, -1);
        dist[s] = 0;
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> q;
        q.emplace(0, s);
        while (!q.empty()) {
            auto [d, v] = q.top();
            q.pop();
            if (d > dist[v])
                continue;
            for (int e : graph[v]) {
                if (res(e) == 0) continue;
                int to = edges[e].v ^ edges[e].u ^ v;
                if (dist[v] + edges[e].w < dist[to]) {
                    dist[to] = dist[v] + edges[e].w;
                    par[to] = v;
                    q.emplace(dist[to], to);
                }
            }
        }
        return dist[f] != INF;
    }

    int dfs(int v, int val) {
        if (val == 0) return 0;
        if (v == f) return val;
        for (int& pos = ptr[v]; pos < graph[v].size(); ++pos) {
            int e = graph[v][pos];
            int to = edges[e].v ^ edges[e].u ^ v;
            if (par[to] != v)
                continue;
            if (dist[to] != dist[v] + edges[e].w || res(e) == 0)
                continue;
            int pushed = dfs(to, min(val, res(e)));
            if (pushed == 0)
                continue;
            edges[e].f += pushed;
            edges[e ^ 1].f -= pushed;
            return pushed;
        }
        return 0;
    }

    pair<int, int> flow() {
        int fl = 0;
        while (spfa()) {
            ptr.assign(n, 0);
            while (int pushed = dfs(s, INF)) {
                fl += pushed;
            }
        }
        int cost = 0;
        for (auto& e : edges) {
            cost += e.f * e.w;
        }
        cost /= 2;
        return {fl, cost};
    }

    vector<int> simple_decomposition(int start) {
        set<int> P;
        vector<int> Q;
        int v = start;
        while (!P.count(v)) {
            if (v == f)
                break;
            int ee = -1;
            for (int e : graph[v]) {
                if (edges[e].f > 0) {
                    ee = e;
                    break;
                }
            }
            if (ee == -1)
                return {};
            Q.push_back(ee);
            P.insert(v);
            v = edges[ee].v ^ edges[ee].u ^ v;
        }
        if (P.count(v)) {
            int idx = -1;
            for (int i = 0; i < Q.size(); ++i) {
                if (edges[i].u == v) {
                    idx = i;
                    break;
                }
            }
            assert(idx != -1);
            Q.erase(Q.begin(), Q.begin() + idx);
        }
        for (int e : Q) {
            --edges[e].f;
            ++edges[e ^ 1].f;
        }
        return Q;
    }

    bool cycle(const vector<int>& p) {
        assert(!p.empty());
        return edges[p[0]].u == edges[p.back()].v;
    }

    vector<vector<int>> full_decomposition() {
        vector<vector<int>> d;
        auto p = simple_decomposition(s);
        while (!p.empty()) {
            if (!cycle(p))
                d.push_back(p);
            p = simple_decomposition(s);
        }
        for (int u = 0; u < n; ++u) {
            p = simple_decomposition(u);
            while (!p.empty()) {
                if (!cycle(p))
                    d.push_back(p);
                p = simple_decomposition(u);
            }
        }
        return d;
    }
};

void solve() {
    int n, m, k; cin >> n >> m >> k;
    int s = 0, t = n;
    MCMF mcmf(n + 1, s, t);
    for (int i = 0; i < m; ++i) {
        int a, b, c; cin >> a >> b >> c;
        assert(a != b);
        mcmf.add_edge(a, b, 1, c);
        mcmf.add_edge(b, a, 1, c);
    }
    mcmf.add_edge(s, 1, k, 0);
    auto [flow, cost] = mcmf.flow();
    if (flow < k) {
        cout << "-1\n";
        return;
    }
    cout << fixed << (double)cost/k << "\n";
    for (int i = 0; i < m * 4; i += 4) {
        int a = mcmf.edges[i].f;
        int b = mcmf.edges[i + 2].f;
        int mn = min(a, b);

        mcmf.edges[i].f -= mn;
        mcmf.edges[i ^ 1].f += mn;

        mcmf.edges[i + 2].f -= mn;
        mcmf.edges[(i + 2) ^ 1].f += mn;
    }

    auto ans = mcmf.full_decomposition();
    for (auto& path : ans) {
        path.erase(path.begin());
        cout << path.size() << " ";
        for (int e : path) {
            cout << e / 4 + 1 << " ";
        }
        cout << "\n";
    }
}

int32_t main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.precision(20);

    solve();
}