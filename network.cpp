#ifndef NETWORK_CPP
#define NETWORK_CPP

#include "network.h"

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

#endif