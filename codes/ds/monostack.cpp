template<typename T>
class MonoStack {
private:
    stack<T> st;

public:
    void push(const T& x)
    {
        while (not st.empty() and st.top() > x)
            st.pop();

        st.emplace(x);
    }

    void pop() { st.pop(); };
    auto top() const { return st.top(); }
    bool empty() const { return st.empty(); }
    size_t size() const { return st.size(); }
};

template<typename T>
ostream& operator<<(ostream& os, const MonoStack<T>& ms)
{
    auto temp(ms);

    while (not temp.empty())
    {
        cout << temp.top() << ' ';
        temp.pop();
    }

    return os;
}
