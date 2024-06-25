#include <bits/stdc++.h>
#define int long long
using namespace std;

mt19937 gen(chrono::steady_clock::now().time_since_epoch().count());

int mul(int a, int b, int mod) {
    return ((__int128)a * (__int128)b) % mod;
}

int binpow(int a, int p, int mod) {
    if (!p) return 1;
    if (p & 1) return mul(a, binpow(a, p - 1, mod), mod);
    auto t = binpow(a, p >> 1, mod);
    return mul(t, t, mod);
}

bool fermat(int n) {
    if (n == 1) return false;
    if (n == 2 || n == 3) return true;
    int ITERS = 30;
    while (ITERS--) {
        int a = gen() % (n - 1) + 1;
        if (__gcd(a, n) != 1)
            return false;
        if (binpow(a, n - 1, n) != 1)
            return false;
    }
    return true;
}

bool miller_rabin(int n) {
    if (n == 1) return false;
    if (n == 2 || n == 3) return true;
    int ITERS = 30;
    int s = 0;
    int d = n - 1;
    while (d % 2 == 0) {
        d /= 2;
        ++s;
    }
    while (ITERS--) {
        int a = gen() % (n - 3) + 2;
        int x = binpow(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        bool flag = false;
        for (int i = 0; i < s - 1; ++i) {
            x = mul(x, x, n);
            if (x == 1) return false;
            if (x == n - 1) {
                flag = true;
                break;
            }
        }
        if (!flag) {
            return false;
        }
    }
    return true;
}

bool is_prime(int x) {
    return fermat(x) && miller_rabin(x);
}

void solve() {
    int n; cin >> n;
    for (int i = 0; i < n; ++i) {
        int x; cin >> x;
        if (is_prime(x)) cout << "YES\n";
        else cout << "NO\n";
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