#include <iostream>
#include <stdio.h>
#include "examples.h"
#include "sort_Cipher.h"
#include "relu.h"
#include <ctime>

using namespace std;
using namespace seal;

void relu_taylor()
{   
    double coeff9 = 0.0000000001;
    double coeff7 = 0.0000000001;
    double coeff5 = 0.0000000001;
    double coeff3 = -125000/12;
    double coeff1 = 12.5;
    
    int iter_time=0;
    int bit=36;
    double a;
    double result;
    double error=0.0;

    clock_t startTime,endTime;
    startTime = clock();

    for(int i=0;i<10;i++)
    {
        a=0.0001*(rand() % 100 / (double)101);
        result = Relu(iter_time, bit, a, coeff9, coeff7, coeff5, coeff3, coeff1);
        error+=(abs(result-a))/a;
    }

    endTime = clock();
    cout << "error: " << error <<endl;
    cout << "The run time is: " <<(double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
    
    cout<<endl;
    
}