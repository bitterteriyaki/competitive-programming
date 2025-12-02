#include <bits/stdc++.h>

using namespace std;

template<typename T>
struct Point {
    T x, y;

    bool operator<(const Point& p) const {
        if (x == p.x)
            return y < p.y;

        return x < p.x;
    }

};

template<typename T>
double dist(const Point<T>& p, const Point<T>& q) {
    return hypot(p.x - q.x, p.y - q.y);
}

template<typename T>
auto closest_pair(int N, vector<Point<T>>& ps) {
    sort(ps.begin(), ps.end());

    auto d = dist(ps[0], ps[1]);
    auto closest = make_pair(ps[0], ps[1]);

    set<Point<T>> box;
    box.emplace(ps[0].y, ps[0].x);
    box.emplace(ps[1].y, ps[1].x);

    for (int i = 2; i < N; ++i) {
        auto p = ps[i];
        auto it = box.lower_bound(Point<T>{p.y - d, 0});

        while (it != box.end()) {
            auto q = Point<T>{it->y, it->x};

            if (q.x < p.x - d) {
                it = box.erase(it);
                continue;
            }

            if (q.y > p.y + d)
                break;

            auto t = dist(p, q);

            if (t < d) {
                d = t;
                closest = make_pair(p, q);
            }

            ++it;
        }

        box.emplace(p.y, p.x);
    }

    return closest;
}
