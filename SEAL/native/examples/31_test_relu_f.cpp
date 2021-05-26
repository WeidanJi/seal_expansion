#include <iostream>
#include <stdio.h>
#include "examples.h"
#include "relu.h"
#include "sort_Cipher.h"
#include "test.h"
#include <ctime>

using namespace std;
using namespace seal;

double Test_ReLU_f(int iter_time, double a,double b,double w0_0,double w0_1,double b0,double w1,double b1,double hide)
{
    int bit = 36;
    EncryptionParameters parms(scheme_type::ckks);

    // params.
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

    // encode,encrypt
    Plaintext w0_0_plain,w0_1_plain,b0_plain,w1_plain,b1_plain;
    encoder.encode(w0_0,scale,w0_0_plain);
    encoder.encode(w0_1,scale,w0_1_plain);
    encoder.encode(b0,scale,b0_plain);
    encoder.encode(w1,scale,w1_plain);
    encoder.encode(b1,scale,b1_plain);
    Plaintext a_plain,b_plain;
    encoder.encode(a,scale,a_plain);
    encoder.encode(b,scale,b_plain);
    Plaintext hide_plain;
    encoder.encode(hide,scale,hide_plain);
    Plaintext add_plain;
    encoder.encode(2.0,scale,add_plain);

    Ciphertext w0_0_cipher,w0_1_cipher,b0_cipher,w1_cipher,b1_cipher;
    encryptor.encrypt(w0_0_plain,w0_0_cipher);
    encryptor.encrypt(w0_1_plain,w0_1_cipher);
    encryptor.encrypt(b0_plain,b0_cipher);
    encryptor.encrypt(w1_plain,w1_cipher);
    encryptor.encrypt(b1_plain,b1_cipher);
    Ciphertext a_cipher,b_cipher;
    encryptor.encrypt(a_plain,a_cipher);
    encryptor.encrypt(b_plain,b_cipher);
    Ciphertext hide_cipher;
    encryptor.encrypt(hide_plain,hide_cipher);
    Ciphertext add_cipher;
    encryptor.encrypt(add_plain,add_cipher);

    //server
    //aw0
    Ciphertext aw00_cipher;
    evaluator.multiply(a_cipher, w0_0_cipher, aw00_cipher);
    evaluator.relinearize_inplace(aw00_cipher, relin_keys);
    evaluator.rescale_to_next_inplace(aw00_cipher);

    //bw0
    Ciphertext bw01_cipher;
    evaluator.multiply(b_cipher, w0_1_cipher, bw01_cipher);
    evaluator.relinearize_inplace(bw01_cipher, relin_keys);
    evaluator.rescale_to_next_inplace(bw01_cipher);

    //aw0+bw0
    aw00_cipher.scale()=scale;
    bw01_cipher.scale()=scale;
    parms_id_type last_parms_id = aw00_cipher.parms_id();
    evaluator.mod_switch_to_inplace(bw01_cipher, last_parms_id);
    evaluator.add_inplace(aw00_cipher, bw01_cipher);

    //aw0+bw0+b0
    aw00_cipher.scale()=scale;
    b0_cipher.scale()=scale;
    last_parms_id = aw00_cipher.parms_id();
    evaluator.mod_switch_to_inplace(b0_cipher, last_parms_id);
    evaluator.add_inplace(aw00_cipher,b0_cipher);

    //relu_f
    double coeff9 = 0.2734375;
    double coeff7 = -1.40625;
    double coeff5 = 2.953125;
    double coeff3 = -3.28125;
    double coeff1 = 2.4609375;

    Plaintext plain_coeff9, plain_coeff7, plain_coeff5, plain_coeff3, plain_coeff1, plain_coeff0;
    encoder.encode(coeff9, scale, plain_coeff9);
    encoder.encode(coeff7, scale, plain_coeff7);
    encoder.encode(coeff5, scale, plain_coeff5);
    encoder.encode(coeff3, scale, plain_coeff3);
    encoder.encode(coeff1, scale, plain_coeff1);
    Plaintext plain_coeff;
    encoder.encode(0.5, scale, plain_coeff);
    Plaintext plain_0;
    encoder.encode(0.0,scale,plain_0);
    Ciphertext cipher_0;
    encryptor.encrypt(plain_0,cipher_0);
    vector<Ciphertext> cipher_result;
    cipher_result = MaxMinOfTwoCipher(iter_time,
        bit, scale, relin_keys, plain_coeff, aw00_cipher, cipher_0, plain_coeff9, plain_coeff7, plain_coeff5, plain_coeff3,
        plain_coeff1);
    
    Ciphertext relu_result;
    relu_result = cipher_result[0];
    
    //(relu(aw0+bw0+b0))*w1
    last_parms_id = relu_result.parms_id();
    evaluator.mod_switch_to_inplace(w1_cipher, last_parms_id);
    relu_result.scale()=scale;
    w1_cipher.scale()=scale;
    Ciphertext abw1_cipher;
    evaluator.multiply(relu_result, w1_cipher, abw1_cipher);
    evaluator.relinearize_inplace(abw1_cipher, relin_keys);
    evaluator.rescale_to_next_inplace(abw1_cipher);

    //(relu(aw0+bw0+b0))*w1*hide
    last_parms_id = abw1_cipher.parms_id();
    evaluator.mod_switch_to_inplace(hide_cipher, last_parms_id);
    abw1_cipher.scale()=scale;
    hide_cipher.scale()=scale;
    Ciphertext abw1_hide_cipher;
    evaluator.multiply(abw1_cipher, hide_cipher, abw1_hide_cipher);
    evaluator.relinearize_inplace(abw1_hide_cipher, relin_keys);
    evaluator.rescale_to_next_inplace(abw1_hide_cipher);

    //(relu(aw0+bw0+b0))*w1*hide+b1
    abw1_hide_cipher.scale()=scale;
    b1_cipher.scale()=scale;
    last_parms_id = abw1_hide_cipher.parms_id();
    evaluator.mod_switch_to_inplace(b1_cipher, last_parms_id);
    evaluator.add_inplace(abw1_hide_cipher, b1_cipher);

    //2*[(relu(aw0+bw0+b0))*w1*hide+b1]
    abw1_hide_cipher.scale()=scale;
    add_cipher.scale()=scale;
    last_parms_id = abw1_hide_cipher.parms_id();
    evaluator.mod_switch_to_inplace(add_cipher, last_parms_id);
    Ciphertext result_cipher;
    evaluator.multiply(abw1_hide_cipher, add_cipher, result_cipher);
    evaluator.relinearize_inplace(result_cipher, relin_keys);
    evaluator.rescale_to_next_inplace(result_cipher);

    //local 
    //decrypt,decode
    Plaintext result_plain;
    decryptor.decrypt(result_cipher,result_plain);
    vector<double> result;
    encoder.decode(result_plain,result);

    double res;
    res=result[0];
    return res;
}

void test_relu_f()
{
    double w0_0=0.36039;
    double w0_1=0.36043;
    double b0=-0.02264;
    double w1=0.03468;
    double b1=0.03141;
    double hide=40;
    int iter_time;

    double a;
    double b;
    double res;
    double relu_res;
    double act_res;
    double ab_res;
    double error;

    clock_t startTime,endTime;
    
    
    for(iter_time=0;iter_time<4;iter_time++)
    {
        startTime = clock();
        error=0.0;
        for(int i=0;i<10;i++)
        {
            a=rand() % 100 / (double)101;
            b=rand() % 100 / (double)101;
            cout<<"a: "<<a<<endl;
            cout<<"b: "<<b<<endl;
            res=Test(a,b,w0_0,w0_1,b0,w1,b1,hide);
            cout<<"predict result: "<<res<<endl;
            relu_res=Test_ReLU_f(iter_time, a,b,w0_0,w0_1,b0,w1,b1,hide);
            cout<<"predict result with relu: "<<relu_res<<endl;
            act_res=2*((w0_0*a+w0_1*b+b0)*w1*hide+b1);
            cout<<"actual result: "<<act_res<<endl;
            ab_res=a+b;
            cout<<"a+b: "<<ab_res<<endl;
            error+=abs(relu_res-ab_res)/ab_res;
        }

        cout<<"iter time: "<<iter_time<<endl;
        endTime = clock();
        cout << "error: " << error <<endl;
        cout << "The run time is: " <<(double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
    
    }
    
    cout<<endl;
}