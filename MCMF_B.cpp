#include <bits/stdc++.h>

#define int long long
using namespace std;

const int INF = 1e18;

pair<int, vector<int>> hungarian(vector<vector<int>> a, int n) {
    vector<int> u(n + 1, 0), v(n + 1, 0), way(n + 1, 0);
    vector<int> p(n + 1);
    for (int i = 1; i <= n; ++i) {
        vector<int> minv(n + 1, INF);
        vector<bool> used(n + 1, false);
        p[0] = i;
        int j0 = 0;
        while (true) {
            used[j0] = true;
            int i0 = p[j0];
            assert(i0 != 0);
            int delta = INF, j1;
            for (int j = 1; j <= n; ++j) {
                if (!used[j]) {
                    if (a[i0][j] - u[i0] - v[j] < minv[j]) {
                        minv[j] = a[i0][j] - u[i0] - v[j];
                        way[j] = j0;
                    }
                    if (minv[j] < delta) {
                        delta = minv[j];
                        j1 = j;
                    }
                }
            }
            for (int j = 0; j <= n; ++j) {
                if (used[j]) {
                    u[p[j]] += delta;
                    v[j] -= delta;
                } else {
                    minv[j] -= delta;
                }
            }
            j0 = j1;
            if (p[j0] == 0)
                break;
        }
        while (true) {
            int j1 = way[j0];
            p[j0] = p[j1];
            j0 = j1;
            if (j0 == 0)
                break;
        }
    }

    vector<int> ans(n + 1);
    for (int j = 1; j <= n; ++j)
        ans[p[j]] = j;
    return {-v[0], ans};
}

void solve() {
    int n;
    cin >> n;
    vector<vector<int>> a(n + 1, vector<int>(n + 1, 0));
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            cin >> a[i][j];
        }
    }
    auto[cost, ans] = hungarian(a, n);
    cout << cost << "\n";
    for (int i = 1; i <= n; ++i)
        cout << i << " " << ans[i] << "\n";
}

int32_t main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
}