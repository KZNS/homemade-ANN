#include <cstdio>
#include "network.h"
#include "dataset.h"
using namespace std;

Dataset x_train, y_train, x_test, y_test;
int main()
{
    //readin data
    x_train.readin_images("data/train-images-idx3-ubyte");
    x_train.datas[0].print_bool(cout);
    y_train.readin_labels("data/train-labels-idx1-ubyte");
    cout << y_train.datas[0];
    x_test.readin_images("data/t10k-images-idx3-ubyte");
    x_test.datas[0].print_bool(cout);
    y_test.readin_labels("data/t10k-labels-idx1-ubyte");
    cout << y_test.datas[0];

    x_train.reshape(-1, 1);
    x_test.reshape(-1, 1);
    printf("%d %d\n", x_train.datas[0].row, x_train.datas[0].col);
    printf("%d %d\n", x_test.datas[0].row, x_test.datas[0].col);

    //init network
    Network net;
    net.set_input_shape(28 * 28);
    net.add_layer(128);
    net.add_layer(10);
    for (int i = 0; i < net.shape.size(); i++)
    {
        cout << net.shape[i] << endl;
    }
    net.init();
    //fit
    net.fit(x_train, y_train, 32, 5);
    //test
    double test_loss, test_acc;
    net.evaluate(x_test, y_test, test_loss, test_acc);
    return 0;
}