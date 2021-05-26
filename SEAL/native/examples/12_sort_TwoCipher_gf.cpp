#include <iostream>
#include <stdio.h>
#include "examples.h"
#include "sort_Cipher.h"
#include <ctime>

using namespace std;
using namespace seal;

vector<Ciphertext> MaxMinOfTwoCipher_gf(int iter_time_g, int iter_time_f,
    int bit, double scale, RelinKeys relin_keys, Plaintext plain_coeff, Ciphertext x1, Ciphertext x2,
    Plaintext plain_coeff9f, Plaintext plain_coeff7f, Plaintext plain_coeff5f, Plaintext plain_coeff3f,
    Plaintext plain_coeff1f, Plaintext plain_coeff9g, Plaintext plain_coeff7g, Plaintext plain_coeff5g, Plaintext plain_coeff3g,
    Plaintext plain_coeff1g)
{
    EncryptionParameters parms(scheme_type::ckks);

    cout<<"MaxMinOfTwoCipher"<<endl;
    // params.
    size_t poly_modulus_degree = 16384 * 2;
    parms.set_poly_modulus_degree(poly_modulus_degree);
    parms.set_coeff_modulus(
        CoeffModulus::Create(poly_modulus_degree, { 60,  bit, bit, bit, bit, bit, bit, bit, bit, bit, bit, bit,
                                                    bit, bit, bit, bit, bit, bit, bit, bit, bit, bit, 60 }));
    SEALContext context(parms);
    //print_parameters(context);
    Evaluator evaluator(context);

    // calculate the diff,sum
    Ciphertext x1_encrypted;
    x1.scale()=scale;
    x2.scale()=scale;
    parms_id_type x1_parms = x1.parms_id();
    evaluator.mod_switch_to_inplace(x2,x1_parms);

    evaluator.sub(x1, x2, x1_encrypted);
    Ciphertext sum;
    evaluator.add(x1, x2, sum);

    
    // calculate sgn(diff)
    Ciphertext iterResult_g;
    iterResult_g = iter(
        iter_time_g, bit, scale, relin_keys, x1_encrypted, plain_coeff9g, plain_coeff7g, plain_coeff5g, plain_coeff3g, plain_coeff1g);

    Ciphertext iterResult_gf;
    iterResult_gf = iter(
        iter_time_f, bit, scale, relin_keys, iterResult_g, plain_coeff9f, plain_coeff7f, plain_coeff5f, plain_coeff3f, plain_coeff1f);

    // calculate max,min
    vector<Ciphertext> cipher_result;
    cipher_result = MaxMin(bit, scale, relin_keys, plain_coeff, sum, x1_encrypted, iterResult_gf);

    return cipher_result;
}

vector<double> sort_2Cipher_gf(int iter_time_g, int iter_time_f, int bit, vector<double> x, double coeff9f, double coeff7f, double coeff5f, double coeff3f, double coeff1f, double coeff9g, double coeff7g, double coeff5g, double coeff3g, double coeff1g)
{
    EncryptionParameters parms(scheme_type::ckks);

    // params.
    cout<<"sort_2Cipher_gf"<<endl;
    size_t poly_modulus_degree = 16384 * 2;
    parms.set_poly_modulus_degree(poly_modulus_degree);
    parms.set_coeff_modulus(
        CoeffModulus::Create(poly_modulus_degree, { 60,  bit, bit, bit, bit, bit, bit, bit, bit, bit, bit, bit,
                                                    bit, bit, bit, bit, bit, bit, bit, bit, bit, bit, 60 }));
    SEALContext context(parms);
    //print_parameters(context);
    cout << endl;

    // local
    // keygen
    KeyGenerator keygen(context);
    auto secret_key = keygen.secret_key();
    PublicKey public_key;
    keygen.create_public_key(public_key);
    RelinKeys relin_keys;
    keygen.create_relin_keys(relin_keys);
    Encryptor encryptor(context, public_key);
    Evaluator evaluator(context);
    Decryptor decryptor(context, secret_key);
    CKKSEncoder encoder(context);
    double scale = pow(2.0, bit);

    // encode,encrypt two numbers
    Plaintext inputp1;
    Plaintext inputp2;
    encoder.encode(x[0], scale, inputp1);
    encoder.encode(x[1], scale, inputp2);

    Ciphertext inputc1;
    Ciphertext inputc2;
    encryptor.encrypt(inputp1, inputc1);
    encryptor.encrypt(inputp2, inputc2);

    Plaintext plain_coeff9f, plain_coeff7f, plain_coeff5f, plain_coeff3f, plain_coeff1f;
    encoder.encode(coeff9f, scale, plain_coeff9f);
    encoder.encode(coeff7f, scale, plain_coeff7f);
    encoder.encode(coeff5f, scale, plain_coeff5f);
    encoder.encode(coeff3f, scale, plain_coeff3f);
    encoder.encode(coeff1f, scale, plain_coeff1f);

    Plaintext plain_coeff9g, plain_coeff7g, plain_coeff5g, plain_coeff3g, plain_coeff1g;
    encoder.encode(coeff9g, scale, plain_coeff9g);
    encoder.encode(coeff7g, scale, plain_coeff7g);
    encoder.encode(coeff5g, scale, plain_coeff5g);
    encoder.encode(coeff3g, scale, plain_coeff3g);
    encoder.encode(coeff1g, scale, plain_coeff1g);

    Plaintext plain_coeff;
    encoder.encode(0.5, scale, plain_coeff);

    // server
    // calculate max,min of two ciphertext
    vector<Ciphertext> cipher_result;
    cipher_result = MaxMinOfTwoCipher_gf(iter_time_g, iter_time_f,
        bit, scale, relin_keys, plain_coeff, inputc1, inputc2, plain_coeff9f, plain_coeff7f, plain_coeff5f, plain_coeff3f,
        plain_coeff1f, plain_coeff9g, plain_coeff7g, plain_coeff5g, plain_coeff3g, plain_coeff1g);

    // local
    // decrypt,decode
    Ciphertext max_result;
    Ciphertext min_result;
    max_result = cipher_result[0];
    min_result = cipher_result[1];

    vector<Plaintext> plain_result;
    Plaintext plain_result_max;
    Plaintext plain_result_min;
    decryptor.decrypt(max_result, plain_result_max);
    decryptor.decrypt(min_result, plain_result_min);
    plain_result.emplace_back(plain_result_max);
    plain_result.emplace_back(plain_result_min);

    vector<double> max_min;
    vector<double> max;
    vector<double> min;
    encoder.decode(plain_result_max, max);
    encoder.decode(plain_result_min, min);
    max_min.push_back(max[0]);
    max_min.push_back(min[0]);

    return max_min;
}


void sort_TwoCipher_gf()
{
    double a;
    double b;
    

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

    int iter_time_g=1;
    int iter_time_f=1;
    int bit=36;
    vector<double> output;
    double error=0.0;

    clock_t startTime,endTime;
    startTime = clock();
    
    for(int i=0;i<10;i++)
    {
        a = rand() % 100 / (double)101;
        b = rand() % 100 / (double)101;
        vector<double> input;
        input.push_back(a);
        input.push_back(b);
        output = sort_2Cipher_gf(iter_time_g, iter_time_f, bit, input, coeff9f, coeff7f, coeff5f, coeff3f, coeff1f, coeff9g, coeff7g, coeff5g, coeff3g, coeff1g);
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