#include <cstdio>
#include "network.h"
#include "dataset.h"
using namespace std;

int main()
{
    //readin data
    Dataset x_train, y_train, x_test, y_test;
    x_train.readin("data/train-images-idx3-ubyte.gz");
    y_train.readin("data/train-labels-idx1-ubyte.gz");
    x_test.readin("data/t10k-images-idx3-ubyte.gz");
    y_test.readin("data/t10k-labels-idx1-ubyte.gz");
    
    //init network
    Network net;
    net.add_layer(28 * 28);
    net.add_layer(128);
    net.add_layer(10);
    //fit
    //test
    return 0;
}