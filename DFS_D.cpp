#include <bits/stdc++.h>

using namespace std;

const int N = 1e5 + 2;
vector<int> graph[N];
bool used[N];
vector<pair<int, int>> edges;
int up[N], depth[N];
int ans[N];
vector<int> stak;
int nxt = 1;

void pop(int sz) {
    while (stak.size() > sz) {
        ans[stak.back()] = nxt;
        stak.pop_back();
    }
    ++nxt;
}

void dfs(int v, int par_id) {
    used[v] = true;
    up[v] = depth[v];
    stak.push_back(v);
    for (int edge_id : graph[v]) {
        int to = v ^ edges[edge_id].first ^ edges[edge_id].second;
        if (edge_id == par_id) continue;
        if (used[to]) {
            up[v] = min(up[v], depth[to]);
        } else {
            depth[to] = depth[v] + 1;
            int sz = stak.size();
            dfs(to, edge_id);
            if (up[to] > depth[v]) {
                pop(sz);
            }
            up[v] = min(up[v], up[to]);
        }
    }
}

void solve() {
    int n, m; cin >> n >> m;
    for (int i = 0; i < m; ++i) {
        int a, b; cin >> a >> b;
        --a, --b;
        graph[a].push_back(edges.size());
        graph[b].push_back(edges.size());
        edges.emplace_back(a, b);
    }
    for (int i = 0; i < n; ++i) {
        if (!used[i]) {
            dfs(i, -1);
            pop(0);
        }
    }
    cout << nxt - 1 << "\n";
    for (int i = 0; i < n; ++i)
        cout << ans[i] << " ";
}

int32_t main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.precision(12);

    solve();
}