#include <bits/stdc++.h>

using namespace std;

struct node {
    node* to[26];
    node* go[26];
    node* suf_link = nullptr;
    node* parent = nullptr;
    char how;
    bool visited = false;
    vector<int> idx;
    int add = 0;

    node() {
        fill(begin(to), end(to), nullptr);
        fill(begin(go), end(go), nullptr);
    }
};

node* root;
vector<node*> nodes;
void add(string& s, int idx) {
    node* v = root;
    for (char c : s) {
        if (!v->to[c - 'a']) {
            v->to[c - 'a'] = new node();
            nodes.push_back(v->to[c - 'a']);
            v->to[c - 'a']->parent = v;
            v->to[c - 'a']->how = c;
        }
        v = v->to[c - 'a'];
    }
    v->idx.push_back(idx);
}

node* get_suf_link(node*);

node* get_go(node* v, char c) {
    if (v->go[c - 'a'])
        return v->go[c - 'a'];
    if (v->to[c - 'a'])
        return v->go[c - 'a'] = v->to[c - 'a'];
    if (v == root)
        return v->go[c - 'a'] = root;
    node* tmp = get_suf_link(v);
    node* lol = get_go(tmp, c);
    return v->go[c - 'a'] = lol;
}

node* get_suf_link(node* v) {
    if (v->suf_link)
        return v->suf_link;
    if (v == root || v->parent == root)
        return v->suf_link = root;
    node* tmp = get_suf_link(v->parent);
    node* lol = get_go(tmp, v->how);
    return v->suf_link = lol;
}

void dfs(node* v, vector<node*>& order) {
    v->visited = true;
    if (!get_suf_link(v)->visited && v != root)
        dfs(get_suf_link(v), order);
    order.push_back(v);
}

void solve() {
    root = new node();
    nodes.push_back(root);
    int n; cin >> n;
    map<string, vector<int>> indexes;
    vector<string> words(n);
    for (int i = 0; i < n; ++i) {
        cin >> words[i];
        indexes[words[i]].push_back(i);
        add(words[i], i);
    }
    string t; cin >> t;
    node* v = root;
    vector<int> ans(n);
    for (char c : t) {
        v = get_go(v, c);
        v->add++;
    }
    vector<node*> order;
    for (auto&& vv : nodes) {
        if (!vv->visited)
            dfs(vv, order);
    }
    reverse(order.begin(), order.end());
    for (auto&& vv : order) {
        if (vv == root)
            continue;
        for (int i : vv->idx) {
            ans[i] = vv->add;
        }
        get_suf_link(vv)->add += vv->add;
    }

    for (int i = 0; i < n; ++i) {
        cout << ans[i] << "\n";
    }
}

int32_t main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
}

/*
5
derere
ere
der
asd
d
drderedredrasaaad
 */