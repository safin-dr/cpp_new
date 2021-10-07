#include <algorithm>
#include <iterator>
#include <omp.h>
#include <memory>
#include <vector>
#include <iostream>
#include <random>

template <typename Iterator>
void mergesort(Iterator from, Iterator to)
{
#pragma omp parallel
    {
#pragma omp single nowait
        static_assert(!std::is_same<typename std::iterator_traits<Iterator>::value_type, void>::value);

        auto n = std::distance(from, to);

        if (1 < n)
        {
#pragma omp task firstprivate (from, to, n)
            {
                Iterator l_from = from;
                Iterator l_to = l_from;
                std::advance(l_to, n / 2);
                mergesort(l_from, l_to);
            }
#pragma omp task firstprivate (from, to, n)
            {
                Iterator r_from = from;
                std::advance(r_from, n / 2);
                Iterator r_to = r_from;
                std::advance(r_to, n - (n / 2));
                mergesort(r_from, r_to);
            }
#pragma omp taskwait

            auto tmp_array = std::make_unique<typename Iterator::value_type[]>(n);
            Iterator l_iter = from;
            Iterator l_end = l_iter;
            std::advance(l_end, n / 2);
            Iterator r_iter = l_end;
            Iterator& r_end = to;

            auto tmp_iter = tmp_array.get();

            while (l_iter != l_end || r_iter != r_end)
            {
                if (*l_iter < *r_iter)
                {
                    *tmp_iter = std::move(*l_iter);
                    ++l_iter;
                    ++tmp_iter;
                }
                else
                {
                    *tmp_iter = std::move(*r_iter);
                    ++r_iter;
                    ++tmp_iter;
                }

                if (l_iter == l_end)
                {
                    std::copy(
                        std::make_move_iterator(r_iter),
                        std::make_move_iterator(r_end),
                        tmp_iter
                    );

                    break;
                }

                if (r_iter == r_end)
                {
                    std::copy(
                        std::make_move_iterator(l_iter),
                        std::make_move_iterator(l_end),
                        tmp_iter
                    );

                    break;
                }
            }

            std::copy(
                std::make_move_iterator(tmp_array.get()),
                std::make_move_iterator(&tmp_array[n]),
                from
            );
        }
    }
}

template<typename value_t>
class node {
public:
    std::pair<int, value_t> x;
    int y;
    node* left = NULL, * right = NULL;

    node(std::pair<int, value_t> x, int y) : x(x), y(y) {}
};

template<typename value_t>
node<value_t>* merge(node<value_t>* l, node<value_t>* r)
{
    if (l == NULL) return r;
    if (r == NULL) return l;

    if (l->y < r->y) {
        r->left = merge(l, r->left);
        return r;
    }
    l->right = merge(l->right, r);
    return l;
}

template<typename value_t>
std::pair<node<value_t>*, node<value_t>*> split(node<value_t>* root, int x)
{
    if (root == NULL) return std::make_pair((node<value_t>*)NULL, (node<value_t>*)NULL);

    if (x < root->x.first) {
        auto temp = split(root->left, x);
        root->left = temp.second;
        return std::make_pair(temp.first, root);
    }
    auto temp = split(root->right, x);
    root->right = temp.first;
    return std::make_pair(root, temp.second);
}

template<typename value_t>
node<value_t>* insert(node<value_t>* root, int x, value_t v)
{
    auto temp = split(root, x);
    return merge(temp.first, merge(new node<value_t>(std::make_pair(x, v), rand()), temp.second));
}

template<typename value_t>
node<value_t>* remove(node<value_t>* root, int x)
{
    auto temp = split(root, x);
    auto temp2 = split(temp.first, x + 1);
    return merge(temp.first, temp2.second);
}

double search(node<double>* root, int x)
{
    if (abs(x - root->x.first) < 0.0001) return root->x.second;
    if (x < root->x.first) return search(root->left, x);
    return search(root->right, x);
}

template<typename value_t>
value_t search(node<value_t>* root, int x)
{
    if (x == root->x.first) return root->x.second;
    if (x < root->x.first) return search(root->left, x);
    return search(root->right, x);
}

template<typename value_t, typename key_t>
class Map
{
private:
    node<value_t>* root;

public:
    Map() : root(NULL) {}

    void push(key_t k, value_t v) {
        root = insert(root, std::hash<key_t>{}(k), v);
    }

    void pop(key_t k) {
        root = remove(root, std::hash<key_t>{}(k));
    }

    value_t operator[](key_t k) {
        return search(root, std::hash<key_t>{}(k));
    }
};

int main()
{
    std::vector<int> v = { 11, 23, 44, 8, 91, 17, 27, 103, 1, 7 };

    mergesort(v.begin(), v.end());

    for (auto i : v) std::cout << i << ' ';
    std::cout << '\n';

    Map<int, int> m;

    m.push(10, 100);
    m.push(1, 1000);
    m.push(12, 1043);

    std::cout << m[1] << ' ' << m[10] << ' ' << m[12] << '\n';
}