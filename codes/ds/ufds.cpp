class UFDS
{
private:
    vector<int> size, ps;

public:
    UFDS(int N) : size(N + 1, 1), ps(N + 1)
    {
        iota(ps.begin(), ps.end(), 0);
    }

    int find_set(int x) const
    {
        return x == ps[x] ? x : find_set(ps[x]);
    }

    bool same_set(int x, int y)
    {
        return find_set(x) == find_set(y);
    }

    void union_set(int x, int y)
    {
        if (same_set(x, y))
            return;

        int p = find_set(x);
        int q = find_set(y);

        if (size[p] < size[q])
            std::swap(p, q);

        ps[q] = p;
        size[p] += size[q];
    }
};
