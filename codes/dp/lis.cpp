const int oo { 2000000010 };

int lis(int N, const vector<int>& xs) {
    vector<int> dp(N + 1, oo);
    dp[0] = -oo;

    auto ans = 0;

    for (int i = 0; i < N; ++i) {
        auto it = lower_bound(dp.begin(), dp.end(), xs[i]);
        auto pos = (int) (it - dp.begin());

        ans = max(ans, pos);
        dp[pos] = xs[i];
    }

    return ans;
}
