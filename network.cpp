#ifndef NETWORK_CPP
#define NETWORK_CPP

#include "network.h"
#include <random>

int Network::random_list(std::vector<int> &ls, int n)
{
    ls.resize(n);
    for (int i = 0; i < n; i++)
    {
        ls[i] = i;
    }
    std::default_random_engine e(std::time(0));
    for (int i = n - 1; i > 0; i--)
    {
        std::uniform_int_distribution<int> u(0, i);
        std::swap(ls[i], ls[u(e)]);
    }
    return 0;
}
Network::Network()
{
    input_shape = 0;
    deep = 0;
}
int Network::set_input_shape(int size)
{
    input_shape = size;
    if (deep)
    {
        w[0].set_shape(shape[0], input_shape);
        w[0].random();
    }
    return 0;
}
int Network::add_layer(int size)
{
    shape.push_back(size);
    deep = shape.size();
    if (deep == 1)
    {
        w.push_back(Matrix(shape[deep - 1], input_shape));
    }
    else
    {
        w.push_back(Matrix(shape[deep - 1], shape[deep - 2]));
    }
    w[deep - 1].random();
    b.push_back(Matrix(shape[deep - 1], 1));
    b[deep - 1].random();
    return 0;
}
int Network::fit(const Dataset &x, const Dataset &y)
{
    std::vector<int> ls;
    random_list(ls, x.size);
    for (int i = 0; i < x.size; i++)
    {
        printf("\r%d/%d", i+1, x.size);
        //fit(x.datas[ls[i]], y.datas[ls[i]]);
    }
    printf("\n");
    return 0;
}
int Network::fit(const Dataset &x, const Dataset &y, int epochs)
{
    for (int i = 1; i <= epochs; i++)
    {
        std::cout << "epoch " << i << std::endl;
        if (fit(x, y))
        {
            std::cout << "error in fit()" << std::endl;
            return -1;
        }
    }
    return 0;
}
int Network::evaluate(const Dataset &x, const Dataset &y, double &test_loss, double &test_acc)
{
    return 0;
}

#endif