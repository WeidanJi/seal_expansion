#include <iostream>
#include "examples.h"
#include <stdio.h>

using namespace std;
using namespace seal;

Ciphertext polynomial(int bit, double scale, RelinKeys relin_keys, Ciphertext x1_encrypted, Plaintext plain_coeff9, Plaintext plain_coeff7, Plaintext plain_coeff5, Plaintext plain_coeff3, Plaintext plain_coeff1);

Ciphertext iter(int times,int bit, double scale, RelinKeys relin_keys, Ciphertext x1_encrypted, Plaintext plain_coeff9, Plaintext plain_coeff7, Plaintext plain_coeff5, Plaintext plain_coeff3, Plaintext plain_coeff1);

vector<Ciphertext> MaxMin(int bit, double scale, RelinKeys relin_keys, Plaintext plain_coeff,Ciphertext y,Ciphertext x,Ciphertext sgnx);

vector<Ciphertext> MaxMinOfTwoCipher(int iter_time, int bit, double scale, RelinKeys relin_keys, Plaintext plain_coeff,Ciphertext x1, Ciphertext x2,Plaintext plain_coeff9,Plaintext plain_coeff7,Plaintext plain_coeff5,Plaintext plain_coeff3,Plaintext plain_coeff1);

vector<double> sort_2Cipher(int iter_time, int bit, vector<double> x, double coeff9, double coeff7, double coeff5, double coeff3, double coeff1);

vector<Ciphertext> MaxMinOfTwoCipher_gf(int iter_time_g, int iter_time_f,
    int bit, double scale, RelinKeys relin_keys, Plaintext plain_coeff, Ciphertext x1, Ciphertext x2,
    Plaintext plain_coeff9f, Plaintext plain_coeff7f, Plaintext plain_coeff5f, Plaintext plain_coeff3f,
    Plaintext plain_coeff1f, Plaintext plain_coeff9g, Plaintext plain_coeff7g, Plaintext plain_coeff5g, Plaintext plain_coeff3g,
    Plaintext plain_coeff1g);
    
vector<double> sort_2Cipher_gf(int iter_time_g, int iter_time_f, int bit, vector<double> x, double coeff9f, double coeff7f, double coeff5f, double coeff3f, double coeff1f, double coeff9g, double coeff7g, double coeff5g, double coeff3g, double coeff1g);