template<typename T>
class MonoQueue {
private:
    deque<T> st;

public:
    void push(const T& x)
    {
        while (not st.empty() and st.back() > x)
            st.pop_back();

        st.emplace_back(x);
    }

    void pop() { st.pop_front(); };
    auto back() const { return st.back(); }
    auto front() const { return st.front(); }
    bool empty() const { return st.empty(); }
    size_t size() const { return st.size(); }
};

template<typename T>
ostream& operator<<(ostream& os, const MonoQueue<T>& ms)
{
    auto temp(ms);

    while (not temp.empty())
    {
        cout << temp.front() << ' ';
        temp.pop();
    }

    return os;
}
