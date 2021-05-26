#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include "math.h"

#define in 2
#define out 1
#define hide 40
using namespace std;
double a[2 * hide];                          //hidden layer result
double a0[2 * hide];                         //before activating
double y = 0;                                //output layer result
double w1[2 * hide][2 * hide], b1[2 * hide]; //hidden layer w
double w2[2 * hide][2 * hide], b2 = 0;       //output layer w
double delta, delta1[2 * hide];              //output layer error, hidden layer error
double x[1010][3];                           //data
double label[1010];                          //label

//relu
double relu(double x)
{
    if (x > 0)
    {
        return x;
    }
    else
    {
        return 0;
    }
}

//data  x[i][0]:a; x[i][1]:b; label[i]=a+b; x[i][2]=a+b(predict)
void data()
{
    for (int i = 0; i < 1000; i++)
    {
        x[i][0] = rand() % 10000;
        x[i][1] = rand() % 10000;
        label[i] = x[i][0] + x[i][1];
    }
    for (int i = 0; i < 1000; i++)
    {
        x[i][2] = 1.0;
        x[i][0] /= 10000;
        x[i][1] /= 10000;
        label[i] /= 20000;
    }
}

//initialize
void Network()
{
    //memset(point name,value,size)
    memset(a, 0, sizeof(a));
    memset(b1, 0, sizeof(b1));

    for (int i = 0; i < hide; i++)
        for (int j = 0; j < hide; j++)
            w1[i][j] = w2[i][j] = 0.5;
}

//forward
void cp(int t)
{
    y = 0;

    //hidden layer result
    for (int i = 0; i < hide; i++)
    {
        a0[i] = w1[i][0] * x[t][0] + w1[i][1] * x[t][1] + b1[i];
        a[i] = relu(a0[i]);
    }

    //output layer result
    for (int i = 0; i < hide; i++)
        y += w2[0][i] * a[i];
    y += b2;
}

//backward
void bp(int t)
{
    //output layer error
    delta = (label[t] - y);

    //hidden layer error
    for (int i = 0; i < hide; i++)
    {

        delta1[i] = a[i] * (1.0 - a[i]) * w2[0][i] * delta;
    }

    //output layer w
    for (int i = 0; i < hide; i++)
        w2[0][i] += 0.1 * delta * a[i];
    b2 += 0.2 * delta;

    //hidden layer w
    for (int i = 0; i < hide; i++)
    {
        w1[i][0] += 0.1 * delta1[i] * x[t][0];
        w1[i][1] += 0.1 * delta1[i] * x[t][1];
        b1[i] += 0.2 * delta1[i];
    }
}

//train
void train()
{
    for (int k = 1; k <= 1000; k++)
    {
        double err = 0;
        for (int i = 0; i < 1000; i++)
        {
            cp(i);
            err += fabs(label[i] - y);
            bp(i);
        }
        if (k % 100 == 0)
            printf("error: %.5f\n", err);
    }
}

//test
void test()
{
    for (int i = 0; i <= 9; i++)
    {
        x[1000][0] = 1000.0 * i / 10000;
        x[1000][1] = 1000.0 * i / 10000;
        cp(1000);
        printf("predict value：%.5f   actual value：%d   accuracy：%.7f\n", y * 20000, 2000 * i, (20000 * y) / (2000 * i));
    }
}

int main()
{
    a[hide] = 1.0;
    data();
    Network();
    train();
    test();
    freopen("out.txt", "w", stdout);

    for (int i = 0; i < hide; i++)
        printf("%.5f ", w1[i][0]);
    for (int i = 0; i < hide; i++)
        printf("%.5f ", w1[i][1]);
    for (int i = 0; i < hide; i++)
        printf("%.5f ", b1[i]);
    for (int i = 0; i < hide; i++)
        printf("%.5f ", w2[0][i]);
    printf("%.5f", b2);
    return 0;
}