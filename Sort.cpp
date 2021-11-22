#include <algorithm>
#include <cstdlib>
#include <iostream>

void quick_sort(int* mas, int size)
{
    int i = 0, j = size - 1;
    int x = mas[size / 2];
    do
    {
        while (mas[i] < x)
        {
            ++i;
        }
        while (mas[j] > x)
        {
            --j;
        }
        if (i <= j)
        {
            std::swap(mas[i], mas[j]);
            ++i;
            --j;
        }
    } while (i <= j);
    if (j > 0)
    {
        quick_sort(mas, j + 1);
    }
    if (i < size - 1)
    {
        quick_sort(&mas[i], size - i);
    }
}

void print(int* arr, int n) {
    for (int i = 0; i < n; i++) {
        std::cout << arr[i];
    }
    std::cout << "\n";
}

int main() {
    int* arr = new int[10];
    for (int i = 0; i < 10; i++) {
        arr[i] = rand() % 10;
        std::cout << arr[i] << "\t";
    }
    std::cout << "\n";
    quick_sort(arr, 5);
    print(arr, 10);
    quick_sort(arr, 10);
    print(arr, 10);

    return 0;
}