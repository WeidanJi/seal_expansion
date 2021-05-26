#include <iostream>
#include <stdio.h>
#include "examples.h"
#include "sort_Cipher.h"
#include "relu.h"
#include <ctime>

using namespace std;
using namespace seal;

void relu_LS()
{   
    double coeff9 = 0.0000000001;
    double coeff7 = -1.17223712832;
    double coeff5 = 3.84829251834;
    double coeff3 = -4.49920497558;
    double coeff1 = 2.81931;
    
    int iter_time=0;
    int bit=36;
    double a;
    double result;
    double error=0.0;

    clock_t startTime,endTime;
    startTime = clock();

    for(int i=0;i<10;i++)
    {
        a=-rand() % 100 / (double)101;
        result = Relu(iter_time, bit, a, coeff9, coeff7, coeff5, coeff3, coeff1);
        error+=(abs(result-a))/a;
    }

    endTime = clock();
    cout << "error: " << error <<endl;
    cout << "The run time is: " <<(double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
    
    cout<<endl;
    
}