using namespace std;

vector<int> strong_borders(const string &s) {
    int m = s.size(), t = -1;
    vector<int> bs(m + 1, -1);

    for (int i = 1; i <= m; ++i) {
        while (t > -1 and s[t] != s[i - 1])
            t = bs[t];

        ++t;
        bs[i] = (i == m or s[t] != s[i]) ? t : bs[t];
    }

    return bs;
}
