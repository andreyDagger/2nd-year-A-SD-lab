#include <bits/stdc++.h>

#define int __int128
using ll = __int128;

using namespace std;

namespace NTT {
    const int maxn = 1 << 20;
    const int p = 9223372036737335297;
    const int g = 3;
    int R[maxn], tmp[maxn];
    int pm(int a, int b) {
        int res = 1;
        while (b) {
            if (b & 1)
                res = res * a % p;
            a = a * a % p;
            b >>= 1;
        }
        return res;
    }
    void NTT(int * a, int n, int on) {
        for (int i = 0; i < n; i++)
            if (i < R[i])
                swap(a[i], a[R[i]]);
        int wn, u, v;
        for (int i = 1, m = 2; i < n; i = m, m <<= 1) {
            wn = pm(g, (p - 1) / m);
            if (on == -1)
                wn = pm(wn, p - 2);
            for (int j = 0; j < n; j += m) {
                for (int k = 0, w = 1; k < i; k++, w = w * wn % p) {
                    u = a[j + k], v = w * a[i + j + k] % p;
                    a[j + k] = (u + v) % p;
                    a[i + j + k] = (u - v + p) % p;
                }
            }
        }
        if (on == -1)
            for (int i = 0, k = pm(n, p - 2); i < n; i++)
                a[i] = a[i] * k % p;
    }
    vector < int > operator * (vector < int > & A, vector < int > & B) {
        vector < int > C;
        int n = A.size(), m = B.size();
        int l1 = n, l2 = m, L = 0;
        m += n, n = 1;
        while (n <= m)
            n <<= 1, L++;
        for (int i = 0; i < n; i++)
            R[i] = (R[i >> 1] >> 1) | ((i & 1) << (L - 1));
        A.resize(n);
        B.resize(n);
        NTT(A.data(), n, 1);
        NTT(B.data(), n, 1);
        for (int i = 0; i < n; i++)
            tmp[i] = A[i] * B[i] % p;
        NTT(tmp, n, -1);
        return vector < int > (tmp, tmp + l1 + l2 - 1);
    }
}

int read() {
    int32_t x; cin >> x;
    return x;
}

void solve() {
    int n = read();
    vector<int> a(n + 1), b(n + 1);
    for (int i = 0; i <= n; ++i) a[i] = read();
    for (int i = 0; i <= n; ++i) b[i] = read();
    auto c = NTT::operator*(a, b);
    for (int i = 0; i <= 2 * n; ++i) {
        cout << (long long)c[i] << " ";
    }
}

signed main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.precision(20);
//    freopen("output.txt", "w", stdout);

    solve();

    return 0;
}