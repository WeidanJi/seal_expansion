#include <iostream>
#include <stdio.h>
#include "examples.h"
#include "sort_Cipher.h"
#include <ctime>

using namespace std;
using namespace seal;

double Relu(int iter_time, int bit, double a, double coeff9, double coeff7, double coeff5, double coeff3, double coeff1)
{
    double b = 0.0;
    vector<double> input;
    input.push_back(a);
    input.push_back(b);

    double relu_result;
    relu_result = sort_2Cipher(iter_time, bit, input, coeff9, coeff7, coeff5, coeff3, coeff1)[0];

    cout << "a: " << a << endl;
    cout << "relu: " << relu_result << endl;

    return relu_result;
}
void relu_f()
{
    double a=(-1)*rand() % 100 / (double)101;
    
    double coeff9 = 0.2734375;
    double coeff7 = -1.40625;
    double coeff5 = 2.953125;
    double coeff3 = -3.28125;
    double coeff1 = 2.4609375;
    double result;    
    int iter_time;
    int bit =36;
    double error=0.0;

    clock_t startTime,endTime;
    startTime = clock();
    
    for(iter_time=0;iter_time<4;iter_time++)
    {
        for(int i=0;i<10;i++)
        {
        a=(rand() % 100 / (double)101);
        result = Relu(iter_time, bit, a, coeff9, coeff7, coeff5, coeff3, coeff1);
        error+=(abs(result-a))/a;
        }
        
        endTime = clock();
        cout << "iter time: "<< iter_time <<endl;
        cout << "error: " << error <<endl;
        cout << "The run time is: " <<(double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
    }
    
    
    cout<<" "<<endl;
    
}