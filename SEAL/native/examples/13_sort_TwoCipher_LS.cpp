#include <iostream>
#include <stdio.h>
#include "examples.h"
#include "sort_Cipher.h"
#include <ctime>

using namespace std;
using namespace seal;

void sort_TwoCipher_LS()
{
    double a;
    double b;
    double coeff9 = 0.0000000001;
    double coeff7 = -1.17223712832;
    double coeff5 = 3.84829251834;
    double coeff3 = -4.49920497558;
    double coeff1 = 2.81931;
    int iter_time=0;
    int bit=36;
    vector<double> output;
    double error=0.0;

    clock_t startTime,endTime;
    startTime = clock();

    for(int i=0;i<10;i++)
    {
        a=rand() % 100 / (double)101;
        b=rand() % 100 / (double)101;
        vector<double> input;
        input.push_back(a);
        input.push_back(b);
        output = sort_2Cipher(iter_time, bit, input, coeff9, coeff7, coeff5, coeff3, coeff1);
        if(a>b)
        {
            error+=(abs(output[0]-a)+abs(output[1]-b))/(a+b);
        }
        else
        {
            error+=(abs(output[0]-b)+abs(output[1]-a))/(a+b);
        }
        cout << "a: " << a << endl;
        cout << "b: " << b << endl;
        cout << "max: " << output[0] << endl;
        cout << "min: " << output[1] << endl;
    }

    endTime = clock();
    cout << "error: " << error <<endl;
    cout << "The run time is: " <<(double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
    
    cout<<endl;
}