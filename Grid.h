#include <iostream>

#pragma once
template <typename T> class Grid;
template <typename T>
class Grid {
private:
    T* memory;
    size_t x_size, y_size;
public:
    Grid(size_t x, size_t y);
    ~Grid();
    T operator()(size_t x_idx, size_t y_idx) const;
    T& operator()(size_t x_idx, size_t y_idx);
    size_t get_xsize() const;
    size_t get_ysize() const;
    Grid<T>& operator=(T element);
    template <typename T>
    friend std::ostream& operator<<(std::ostream& os, Grid<T> const&);
    template <typename T>
    friend std::istream& operator>>(std::istream& is, Grid<T>&);
};


template<typename T>
Grid<T>::Grid(size_t x, size_t y)
{
    x_size = x;
    y_size = y;
    memory = new T[y_size * x_size];
}

template<typename T>
Grid<T>::~Grid()
{
    delete[] memory;
}

template<typename T>
T Grid<T>::operator()(size_t x_idx, size_t y_idx) const
{
    return memory[y_idx * x_size + x_idx];
}

template<typename T>
T& Grid<T>::operator()(size_t x_idx, size_t y_idx)
{
    return memory[y_idx * x_size + x_idx];
}

template<typename T>
size_t Grid<T>::get_xsize() const
{
    return x_size;
}

template<typename T>
size_t Grid<T>::get_ysize() const
{
    return y_size;
}

template<typename T>
Grid<T>& Grid<T>::operator=(T element)
{
    for (int i = 0; i < y_size * x_size; i++)
    {
        memory[i] = element;
    }
    return *this;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, Grid<T> const& grid)
{
    for (int i = 0; i < grid.y_size; i++)
    {
        for (int j = 0; j < grid.x_size; j++)
        {
            os << grid(j, i) << '\t';
        }
        os << '\n';
    }
    return os;
}

template <typename T>
std::istream& operator>>(std::istream& is, Grid<T>& grid)
{
    for (int i = 0; i < grid.y_size; i++)
    {
        for (int j = 0; j < grid.x_size; j++)
        {
            is >> grid(j, i);
        }
    }
    return is;
}
