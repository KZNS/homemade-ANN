#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include "matrix.h"

class Network
{
private:
    std::vector<Matrix> w, b;

public:
    Network();
    int input_shape;
    std::vector<int> shape;
    int deep;
    int set_input_shape(int size);
    int add_layer(int size);
};

#include "network.cpp"

#endif