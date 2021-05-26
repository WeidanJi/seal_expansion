#include <iostream>
#include <stdio.h>
#include "examples.h"
#include "sort_Cipher.h"
#include <ctime>

using namespace std;
using namespace seal;

double Relu_gf(int iter_time_g, int iter_time_f, int bit, double a, double coeff9f, double coeff7f, double coeff5f, double coeff3f, double coeff1f, double coeff9g, double coeff7g, double coeff5g, double coeff3g, double coeff1g)
{
    double b = 0.0;
    vector<double> input;
    input.push_back(a);
    input.push_back(b);

    double relu_result;
    relu_result = sort_2Cipher_gf(iter_time_g, iter_time_f, bit, input, coeff9f, coeff7f, coeff5f, coeff3f, coeff1f, coeff9g, coeff7g, coeff5g, coeff3g, coeff1g)[0];

    cout << "a: " << a << endl;
    cout << "relu: " << relu_result << endl;

    return relu_result;
}

void relu_gf()
{
    double coeff9f = 0.2734375;
    double coeff7f = -1.40625;
    double coeff5f = 2.953125;
    double coeff3f = -3.28125;
    double coeff1f = 2.4609375;

    double coeff9g = 45.53027;
    double coeff7g = -110.83203125;
    double coeff5g = 94.7412109375;
    double coeff3g = -34.154296875;
    double coeff1g = 5.712890625;

    int iter_time_g;;
    int iter_time_f;

    int bit=36;

    double result;
    double a;
    
    double error=0.0;

    clock_t startTime,endTime;
    startTime = clock();

    for(iter_time_g=0;iter_time_g<3;iter_time_g++)
    {
        for(iter_time_f=0;iter_time_f<3;iter_time_f++)
        {
            for(int i=0;i<10;i++)
            {
                a=(rand() % 100 / (double)101);
                result=Relu_gf(iter_time_g,iter_time_f,bit,a,coeff9f,coeff7f,coeff5f,coeff3f,coeff1f,coeff9g,coeff7g,coeff5g,coeff3g,coeff1g);
                error+=(abs(result-a))/a;
            }

            endTime = clock();
            cout << "iter time g: "<< iter_time_g <<endl;
            cout << "iter time f: "<< iter_time_f <<endl;
            cout << "error: " << error <<endl;
            cout << "The run time is: " <<(double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
    
        }
    }
    
    cout<<endl;    

}