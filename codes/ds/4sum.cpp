using ii = pair<int, int>;

vector<int> _4sum(const vector<int>& xs) {
    unordered_map<int, ii> ps;

    for (auto x : xs)
        for (auto y : xs)
            ps[x + y] = {x, y};

    for (auto [s, p] : ps)
        if (ps.count(-s))
            return {p.first, p.second, ps[-s].first, ps[-s].second};

    return {};
}
