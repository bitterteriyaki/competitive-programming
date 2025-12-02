const int MAX { 1010000 }, oo { 1000000010 };
int st[MAX];

int coin_change(int M, const vector<int>& cs) {
    for (int m = 1; m <= M; ++m)
        st[m] = oo;

    st[0] = 0;

    for (auto c : cs)
        for (int m = c; m <= M; ++m)
            st[m] = min(st[m], st[m - c] + 1);

    return st[M];
}
