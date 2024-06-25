#include <bits/stdc++.h>

using namespace std;

struct edge {
    int u, v;
    int f{0};
    int c;

    edge(int u, int v, int f, int c) : u(u), v(v), f(f), c(c) {}
};

struct Dinic {
    const int INF = 2e9;
    int n, s, f;
    vector<edge> edges;
    vector<vector<int>> graph;
    vector<int> level;
    vector<int> ptr;

    void add_edge(int u, int v, int cap) {
        graph[u].push_back(edges.size());
        graph[v].push_back(edges.size() + 1);
        edges.emplace_back(u, v, 0, cap);
        edges.emplace_back(v, u, 0, 0);
    }

    explicit Dinic(int n, int s, int f) : n(n), s(s), f(f) {
        graph.resize(n);
        level.assign(n, -1);
        ptr.resize(n);
    }

    int res(int e) {
        return edges[e].c - edges[e].f;
    }

    bool bfs() {
        queue<int> q;
        level.assign(n, -1);
        q.push(s);
        level[s] = 0;
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (int e : graph[v]) {
                if (res(e) == 0) continue;
                int to = edges[e].v ^ edges[e].u ^ v;
                if (level[to] != -1) continue;
                level[to] = level[v] + 1;
                q.push(to);
            }
        }
        return level[f] != -1;
    }

    int dfs(int v, int val) {
        if (val == 0) return 0;
        if (v == f) return val;
        for (int& pos = ptr[v]; pos < graph[v].size(); ++pos) {
            int e = graph[v][pos];
            int to = edges[e].v ^ edges[e].u ^ v;
            if (level[to] == -1)
                continue;
            if (level[to] != level[v] + 1 || res(e) == 0)
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

    int flow() {
        int res = 0;
        while (true) {
            if (!bfs())
                break;
            ptr.assign(n, 0);
            while (int pushed = dfs(s, INF)) {
                res += pushed;
            }
        }
        return res;
    }

    vector<bool> minimal_cut() {
        queue<int> q;
        vector<bool> used(n);
        q.push(s);
        used[s] = true;
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (int e : graph[v]) {
                if (res(e) == 0)
                    continue;
                int to = edges[e].u ^ edges[e].v ^ v;
                if (used[to])
                    continue;
                used[to] = true;
                q.push(to);
            }
        }
        return used;
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
            if (d.size() >= 2)
                break;
            p = simple_decomposition(s);
        }
        for (int u = 0; u < n; ++u) {
            p = simple_decomposition(u);
            while (!p.empty()) {
                if (!cycle(p))
                    d.push_back(p);
                if (d.size() >= 2)
                    break;
                p = simple_decomposition(u);
            }
        }
        return d;
    }
};

void solve() {
    int n, m, s, f; cin >> n >> m >> s >> f;
    --s, --f;
    Dinic dinic(n, s, f);
    for (int i = 0; i < m; ++i) {
        int a, b; cin >> a >> b;
        --a, --b;
        dinic.add_edge(a, b, 1);
    }
    int flow = dinic.flow();
    if (flow < 2) {
        cout << "NO\n";
        return;
    }
    vector<vector<int>> paths = dinic.full_decomposition();
    assert(paths.size() >= 2);
    cout << "YES\n";

    auto print_path = [&](const vector<int>& path) {
        cout << dinic.edges[path[0]].u + 1 << " ";
        for (int e : path) {
            cout << dinic.edges[e].v + 1 << " ";
        }
        cout << "\n";
    };

    print_path(paths[0]);
    print_path(paths[1]);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
}