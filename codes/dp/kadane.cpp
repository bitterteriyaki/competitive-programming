int kadane(const vector<int>& xs) {
    vector<int> dp(xs.size());
    dp[0] = xs[0];

    for (size_t i = 1; i < xs.size(); ++i)
        dp[i] = max(xs[i], dp[i - 1] + xs[i]);

    return *max_element(dp.begin(), dp.end());
}
