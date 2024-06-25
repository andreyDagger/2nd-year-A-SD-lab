#include <bits/stdc++.h>

using namespace std;
using ll = long long;
using ull = unsigned long long;

const int P = 31;
const int N = 1e5 + 5;

const int MOD = 1e9 + 7;

struct Hash {
    ull h1;
    ll h2;

    Hash() = default;

    Hash(ull h1, ll h2) : h1(h1), h2(h2) {}

    Hash(int x) : h1(x), h2(x) {}

    friend Hash operator+(const Hash &a, const Hash &b) {
        return {a.h1 + b.h1, (a.h2 + b.h2) % MOD};
    }

    friend Hash operator-(const Hash &a, const Hash &b) {
        return {a.h1 - b.h1, (a.h2 - b.h2 + MOD) % MOD};
    }

    friend Hash operator*(const Hash &a, const Hash &b) {
        return {a.h1 * b.h1, a.h2 * 1ll * b.h2 % MOD};
    }

    friend bool operator==(const Hash &a, const Hash &b) {
        return a.h1 == b.h1 && a.h2 == b.h2;
    }

    friend bool operator!=(const Hash &a, const Hash &b) {
        return !(a == b);
    }

    Hash &operator+=(const Hash &other) {
        h1 += other.h1;
        h2 = (h2 + other.h2) % MOD;
        return *this;
    }

    Hash &operator-=(const Hash &other) {
        h1 -= other.h1;
        h2 = (h2 - other.h2 + MOD) % MOD;
        return *this;
    }

    Hash &operator*=(const Hash &other) {
        h1 *= other.h1;
        h2 = (h2 * 1ll * other.h2) % MOD;
        return *this;
    }
};

Hash pw[N];

struct HashFunction {
    static uint64_t splitmix64(uint64_t x) {
        // http://xorshift.di.unimi.it/splitmix64.c
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }

    size_t operator()(const Hash &h) const {
        static const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(h.h1 ^ h.h2 + FIXED_RANDOM);
    }
};

int code(char c) {
    if (isupper(c)) return c - 'A';
    return c - 'a' + 26;
}

void solve() {
    string q;
    getline(cin, q);
    int n = q.size();
    vector<int> s(q.size());
    for (int i = 0; i < q.size(); ++i) {
        s[i] = (int) q[i];
    }
    vector<int> c(n), p(n);

    vector<vector<int>> pos(130);
    for (int i = 0; i < n; i++)
        pos[s[i]].push_back(i);

    int cnt = 0;
    int classes = 0;
    for (int x = 0; x < pos.size(); ++x) {
        for (int u : pos[x]) {
            c[u] = classes, p[cnt++] = u;
        }
        classes += !pos[x].empty();
    }

    for (int pw2 = 1; (1 << (pw2 - 1)) <= n; pw2++) {
        cnt = 0;
        vector<vector<int> > a(classes);
        vector<int> new_c(n);

        for (int i = 0; i < n; i++) {
            int j = (p[i] - (1 << (pw2 - 1)) + n) % n;
            a[c[j]].push_back(j);
        }

        int new_classes = 0;
        for (int i = 0; i < classes; i++) {
            for (int j = 0; j < a[i].size(); j++) {
                if (j == 0 || c[(a[i][j] + (1 << (pw2 - 1))) % n] != c[(a[i][j - 1] + (1 << (pw2 - 1))) % n]) {
                    new_c[a[i][j]] = new_classes;
                    new_classes++;
                } else {
                    new_c[a[i][j]] = new_classes - 1;
                }
                p[cnt] = a[i][j];
                ++cnt;
            }
        }

        c.swap(new_c);
        classes = new_classes;
    }
    int k; cin >> k;
    --k;
    if (k >= classes) cout << "IMPOSSIBLE\n";
    else {
        int t = 0;
        while (c[p[t]] != k)
            ++t;
        cnt = 0;
        for (int i = p[t]; cnt < n; i = (i + 1) % n, ++cnt)
            cout << q[i];
    }

//    vector<int> lcp(n - 1, 0);
//    vector<int> rev_p(n, 0);
//
//    for (int i = 0; i < n; i++)
//        rev_p[p[i]] = i;
//
//    int k = 0;
//
//    for (int i = 0; i < n; i++) {
//        if (rev_p[i] == n - 1) {
//            k = 0;
//            continue;
//        }
//        int j = p[rev_p[i] + 1];
//        while (i + k < n && j + k < n && s[i + k] == s[j + k])
//            k++;
//        lcp[rev_p[i]] = k;
//        if (k > 0)
//            k--;
//    }

//    cout << n * 1ll * n - n * 1ll * (n - 1) / 2 - accumulate(lcp.begin(), lcp.end(), 0ll) - n << "\n";
}

int32_t main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

//    freopen("suffarray.in", "r", stdin);
//    freopen("suffarray.out", "w", stdout);

    pw[0] = 1;
    for (int i = 1; i < N; ++i) pw[i] = pw[i - 1] * P;
    solve();

    return 0;
}