#ifndef NETWORK_CPP
#define NETWORK_CPP

#include "network.h"

network::~network()
{
    for (int i = 0; i < w.size(); i++)
    {
        delete w[i];
    }
    for (int i = 0; i < b.size(); i++)
    {
        delete b[i];
    }
}
void network::add_layer(int size)
{
    shape.push_back(size);
    deep = shape.size();
    if (deep >= 2)
    {
        w.push_back(new Matrix(shape[deep-2], shape[deep-1]));
        w[w.size()-1]->random();
        b.push_back(new Matrix(shape[deep-1], 1));
        b[b.size()-1]->random();
    }
}


#endif