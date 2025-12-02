template<typename T>
class SegmentTree
{
    int N;
    vector<T> ns;
    function<T(T, T)> op;
    T identity;

public:
    SegmentTree(const vector<T>& xs, function<T(T, T)> operation, T id)
        : N(xs.size()), ns(2*N, id), op(operation), identity(id)
    {
        copy(xs.begin(), xs.end(), ns.begin() + N);

        for (int i = N - 1; i > 0; --i)
            ns[i] = op(ns[2*i], ns[2*i + 1]);
    }

    T query(int i, int j)
    {
        int a = i + N, b = j + N;
        T s = identity;

        while (a <= b)
        {
            if (a & 1)
                s = op(s, ns[a++]);

            if (not (b & 1))
                s = op(s, ns[b--]);

            a /= 2;
            b /= 2;
        }

        return s;
    }

    void update(int i, T value)
    {
        int a = i + N;

        ns[a] = value;

        while (a >>= 1)
            ns[a] = op(ns[2*a], ns[2*a + 1]);
    }
};
