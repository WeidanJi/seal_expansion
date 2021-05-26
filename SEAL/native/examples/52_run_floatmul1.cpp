#include <iostream>
#include <stdio.h>
#include "examples.h"
#include <ctime>

using namespace std;
using namespace seal;

uint64_t test_floatmul1(uint64_t a,uint64_t b)
{

    clock_t startTime,endTime;
    startTime=clock();

    int bit = 40;
    EncryptionParameters parms(scheme_type::ckks);

    // params.
    size_t poly_modulus_degree = 8192;
    parms.set_poly_modulus_degree(poly_modulus_degree);
    parms.set_coeff_modulus(
        CoeffModulus::Create(poly_modulus_degree, { 60, bit,bit,60 }));
    SEALContext context(parms);

    endTime = clock();
    cout << "parms time is: " <<(double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;

    // local
    // keygen
    startTime=clock();

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

    endTime = clock();
    cout << "keygen time is: " <<(double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;

    Plaintext plain_a,plain_b;
    startTime=clock();
    encoder.encode(a,scale,plain_a);
    endTime = clock();
    cout << "encode one time is: " <<(double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
    encoder.encode(b,scale,plain_b);

    Ciphertext cipher_a,cipher_b,cipher_c,cipher_d,cipher_e,cipher_f;
    startTime=clock();
    encryptor.encrypt(plain_a,cipher_a);
    endTime = clock();
    cout << "encrypt one time is: " <<(double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
    encryptor.encrypt(plain_b,cipher_b);
    
    Ciphertext cipher_sum1;
    startTime=clock();
    evaluator.multiply(cipher_a,cipher_b,cipher_sum1);
    endTime = clock();
    cout << "mul one time is: " <<(double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;

    cout<<"cipher size: "<<cipher_sum1.size()*cipher_sum1.poly_modulus_degree()*cipher_sum1.coeff_modulus_size()<<endl;

    Plaintext plain_sum;
    startTime=clock();
    decryptor.decrypt(cipher_sum1,plain_sum);
    endTime = clock();
    cout << "decrypt one time is: " <<(double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;

    vector<double> sum;
    startTime=clock();
    encoder.decode(plain_sum,sum);
    endTime = clock();
    cout << "decode one time is: " <<(double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;

    return sum[0];
}

void run_floatmul1()
{
    uint64_t a=rand() % 100 / (double)101;
    uint64_t b=rand() % 100 / (double)101;
    uint64_t sum=0.0;
    clock_t startTime,endTime;
    startTime=clock();
    sum=test_floatmul1(a,b);
    endTime = clock();
    //cout<<"error: "<<(sum-(a*b))/sum<<endl;
    cout<<endl;
}