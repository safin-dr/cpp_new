#include<iostream>

template <typename T>
class Grid {
private:
    bool* is_sub;
    T* memory;
    Grid* memory1;
    size_t x_size, y_size;

public:
    Grid(size_t x_size = 1, size_t y_size = 1) : x_size(x_size), y_size(y_size) {
        is_sub = new bool[x_size * y_size];
        memory = new T[x_size * y_size];
        memory1 = nullptr;
        for (int i = 0; i < x_size; i++) {
            for (int j = 0; j < y_size; j++) {
                is_sub[i * y_size + j] = false;
            }
        }
    };

    T operator()(size_t x_idx, size_t y_idx) const {
        return memory[x_idx * y_size + y_idx];
    };

    T& operator()(size_t x_idx, size_t y_idx) {
        return memory[x_idx * y_size + y_idx];
    };

    Grid<T>& get_subgrid(size_t x_idx, size_t y_idx) {
        return memory1[x_idx * y_size + y_idx];
    };

    size_t get_xsize() const {
        return x_size;
    };

    size_t get_ysize() const {
        return y_size;
    };

    T* get_memory() const {
        return memory;
    };

    Grid& operator= (T x) {
        for (int i = 0; i < x_size; i++) {
            for (int j = 0; j < y_size; j++) {
                memory[i * y_size + j] = x;
            }
        }
        return *this;
    };

    Grid& operator= (Grid const& g) {
        delete[] memory;
        delete[] memory1;
        delete[] is_sub;
        is_sub = new bool[g.x_size * g.y_size];
        memory = new T[g.x_size * g.y_size];
        if (g.memory1 != nullptr)
            memory1 = new Grid<T>[g.x_size * g.y_size];
        else
            memory1 = nullptr;
        x_size = g.x_size;
        y_size = g.y_size;
        for (int i = 0; i < x_size; i++) {
            for (int j = 0; j < y_size; j++) {
                memory[i * y_size + j] = g.memory[i * y_size + j];
                if (memory1 != nullptr)
                    memory1[i * y_size + j] = g.memory1[i * y_size + j];
                is_sub[i * y_size + j] = g.is_sub[i * y_size + j];
            }
        }
        return *this;
    };

    template<typename P>
    friend std::ostream& operator<<(std::ostream& os, Grid<P> const& g);

    template<typename P>
    friend std::istream& operator>>(std::istream& is, Grid<P>& g);

    Grid<T>& make_subgrid(size_t x_idx, size_t y_idx, size_t x_sub_size, size_t y_sub_size) {
        if (memory1 == nullptr) memory1 = new Grid<T>[x_size * y_size];
        Grid<T> g(x_sub_size, y_sub_size);
        T x = memory[x_idx * y_size + y_idx];
        g = x;
        memory1[x_idx * y_size + y_idx] = g;
        is_sub[x_idx * y_size + y_idx] = true;
        return *this;
    };

    Grid<T>& collapse_subgrid(size_t x_idx, size_t y_idx) {
        T x, summ = 0;
        for (int i = 0; i < memory1[x_idx * y_size + y_idx].x_size; i++) {
            for (int j = 0; j < memory1[x_idx * y_size + y_idx].y_size; j++)
                summ += memory1[x_idx * y_size + y_idx].memory[memory1[x_idx * y_size + y_idx].y_size * i + j];
        }
        x = summ / (memory1[x_idx * y_size + y_idx].y_size * memory1[x_idx * y_size + y_idx].x_size);
        is_sub[x_idx * y_size + y_idx] = false;
        memory[x_idx * y_size + y_idx] = x;
        bool b = false;
        for (int i = 0; i < x_size; i++) {
            for (int j = 0; j < y_size; j++)
                if (is_sub[i * y_size + j]) b = true;
        }
        if (!b) memory1 = nullptr;
        return *this;
    };

    ~Grid() {
        delete[] memory;
        delete[] memory1;
        delete[] is_sub;
    };

    bool is_subgrid(size_t x_idx, size_t y_idx) const {
        return is_sub[x_idx * y_size + y_idx];
    };

};

template <typename V>
std::istream& operator>>(std::istream& is, Grid<V>& g) {
    V x;
    for (int i = 0; i < g.x_size; i++) {
        for (int j = 0; j < g.y_size; j++) {
            is >> x;
            g.memory[i * g.y_size + j] = x;
        }
    }
    return is;
};

template <typename U>
std::ostream& operator<<(std::ostream& os, Grid<U> const& g) {
    for (int i = 0; i < g.get_xsize(); i++) {
        for (int j = 0; j < g.get_ysize(); j++) {
            os << g(i, j) << " ";
        }
        os << "\n";
    }
    if ((g.get_xsize() == 0) && (g.get_ysize() == 0)) os << "Empty";
    return os;
};

int main() {
    Grid<int> g(2, 2);
    std::cin >> g;
    g.make_subgrid(0, 0, 2, 2);
    g.get_subgrid(0, 0).make_subgrid(1, 1, 3, 3);
    std::cout << g.get_subgrid(0, 0);
    std::cout << g.get_subgrid(0, 0).get_subgrid(1, 1);
    g.collapse_subgrid(0, 0);
    std::cout << g;
    return 0;
}