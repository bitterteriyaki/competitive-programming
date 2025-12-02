template<typename T>
class LazySegmentTree
{
private:
    int N;
    vector<T> ns, lazy;
    function<T(T, T)> op;
    function<T(T, int)> range_op;
    T identity;

public:
    LazySegmentTree(
        const vector<T>& xs,
        function<T(T, T)> operation,
        function<T(T, int)> range_operation,
        T id
    )
        : N(xs.size()), ns(4*N, id), lazy(4*N, id),
          op(operation), range_op(range_operation), identity(id)
    {
        for (size_t i = 0; i < xs.size(); ++i)
            update(i, i, xs[i]);
    }

    void update(int a, int b, T value)
    {
        update(1, 0, N - 1, a, b, value);
    }

private:
    void update(int node, int L, int R, int a, int b, T value) {
        if (lazy[node] != identity)
        {
            ns[node] = op(ns[node], range_op(lazy[node], R - L + 1));

            if (L < R)
            {
                lazy[2*node] = op(lazy[2*node], lazy[node]);
                lazy[2*node + 1] = op(lazy[2*node + 1], lazy[node]);
            }

            lazy[node] = identity;
        }

        if (a > R or b < L)
            return;

        if (a <= L and R <= b)
        {
            ns[node] = op(ns[node], range_op(value, R - L + 1));

            if (L < R)
            {
                lazy[2*node] = op(lazy[2*node], value);
                lazy[2*node + 1] = op(lazy[2*node + 1], value);
            }

            return;
        }

        update(2*node, L, (L + R)/2, a, b, value);
        update(2*node + 1, (L + R)/2 + 1, R, a, b, value);

        ns[node] = op(ns[2*node], ns[2*node + 1]);
    }

public:
    T query(int a, int b)
    {
        return query(1, 0, N - 1, a, b);
    }

private:
    T query(int node, int L, int R, int a, int b)
    {
        if (lazy[node] != identity)
        {
            ns[node] = op(ns[node], range_op(lazy[node], R - L + 1));

            if (L < R) {
                lazy[2*node] = op(lazy[2*node], lazy[node]);
                lazy[2*node + 1] = op(lazy[2*node + 1], lazy[node]);
            }

            lazy[node] = identity;
        }

        if (a > R or b < L)
            return identity;

        if (a <= L and R <= b)
            return ns[node];

        T x = query(2*node, L, (L + R)/2, a, b);
        T y = query(2*node + 1, (L + R)/2 + 1, R, a, b);

        return op(x, y);
    }
};
