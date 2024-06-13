using ii = pair<int, int>;
using edge = tuple<int, int, int>;

const int MAX { 100010 };
vector<ii> adj[MAX];

vector<int> dijkstra(int s, int N)
{
    const int oo { 1000000010 };

    vector<int> dist(N + 1, oo);
    dist[s] = 0;

    set<ii> U;
    U.emplace(0, s);

    while (not U.empty())
    {
        auto [d, u] = *U.begin();
        U.erase(U.begin());

        for (auto [v, w] : adj[u])
        {
            if (dist[v] > d + w)
            {
                if (U.count(ii(dist[v], v)))
                    U.erase(ii(dist[v], v));

                dist[v] = d + w;
                U.emplace(dist[v], v);
            }
        }
    }

    return dist;
}
