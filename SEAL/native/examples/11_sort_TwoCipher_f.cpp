#include <iostream>
#include <stdio.h>
#include "examples.h"
#include <ctime>

using namespace std;
using namespace seal;

Ciphertext polynomial(
    int bit, double scale, RelinKeys relin_keys, Ciphertext x1_encrypted, Plaintext plain_coeff9,
    Plaintext plain_coeff7, Plaintext plain_coeff5, Plaintext plain_coeff3, Plaintext plain_coeff1)
{
    EncryptionParameters parms(scheme_type::ckks);

    cout<<"polynomial"<<endl;
    // params.
    size_t poly_modulus_degree = 16384 * 2;
    parms.set_poly_modulus_degree(poly_modulus_degree);
    parms.set_coeff_modulus(
        CoeffModulus::Create(poly_modulus_degree, { 60,  bit, bit, bit, bit, bit, bit, bit, bit, bit, bit, bit,
                                                    bit, bit, bit, bit, bit, bit, bit, bit, bit, bit, 60 }));
    SEALContext context(parms);
    //print_parameters(context);
    Evaluator evaluator(context);

    // input the polynomial
    Ciphertext x9_encrypted_0;
    evaluator.square(x1_encrypted, x9_encrypted_0);
    evaluator.relinearize_inplace(x9_encrypted_0, relin_keys);
    evaluator.rescale_to_next_inplace(x9_encrypted_0);
    Ciphertext x9_encrypted_1;
    evaluator.square(x9_encrypted_0, x9_encrypted_1);
    evaluator.relinearize_inplace(x9_encrypted_1, relin_keys);
    evaluator.rescale_to_next_inplace(x9_encrypted_1);
    Ciphertext x9_encrypted;
    evaluator.square(x9_encrypted_1, x9_encrypted);
    evaluator.relinearize_inplace(x9_encrypted, relin_keys);
    evaluator.rescale_to_next_inplace(x9_encrypted);
    Ciphertext x9_encrypted_coeff9;
    parms_id_type last_parms_id1 = x1_encrypted.parms_id();
    evaluator.mod_switch_to_inplace(plain_coeff9, last_parms_id1);
    evaluator.multiply_plain(x1_encrypted, plain_coeff9, x9_encrypted_coeff9);
    evaluator.rescale_to_next_inplace(x9_encrypted_coeff9);
    x9_encrypted.scale() = scale;
    x9_encrypted_coeff9.scale() = scale;
    parms_id_type last_parms_id9 = x9_encrypted.parms_id();
    evaluator.mod_switch_to_inplace(x9_encrypted_coeff9, last_parms_id9);
    evaluator.multiply_inplace(x9_encrypted, x9_encrypted_coeff9);
    evaluator.relinearize_inplace(x9_encrypted, relin_keys);
    evaluator.rescale_to_next_inplace(x9_encrypted);

    Ciphertext x7_encrypted_0;
    evaluator.square(x1_encrypted, x7_encrypted_0);
    evaluator.relinearize_inplace(x7_encrypted_0, relin_keys);
    evaluator.rescale_to_next_inplace(x7_encrypted_0);
    Ciphertext x7_encrypted;
    evaluator.square(x7_encrypted_0, x7_encrypted);
    evaluator.relinearize_inplace(x7_encrypted, relin_keys);
    evaluator.rescale_to_next_inplace(x7_encrypted);
    Ciphertext x7_encrypted_coeff7;
    evaluator.mod_switch_to_inplace(plain_coeff7, last_parms_id1);
    evaluator.multiply_plain(x1_encrypted, plain_coeff7, x7_encrypted_coeff7);
    evaluator.rescale_to_next_inplace(x7_encrypted_coeff7);
    evaluator.multiply_inplace(x7_encrypted_0, x7_encrypted_coeff7);
    evaluator.relinearize_inplace(x7_encrypted_0, relin_keys);
    evaluator.rescale_to_next_inplace(x7_encrypted_0);
    evaluator.multiply_inplace(x7_encrypted, x7_encrypted_0);
    evaluator.relinearize_inplace(x7_encrypted, relin_keys);
    evaluator.rescale_to_next_inplace(x7_encrypted);

    Ciphertext x5_encrypted_0;
    evaluator.square(x1_encrypted, x5_encrypted_0);
    evaluator.relinearize_inplace(x5_encrypted_0, relin_keys);
    evaluator.rescale_to_next_inplace(x5_encrypted_0);
    Ciphertext x5_encrypted;
    evaluator.square(x5_encrypted_0, x5_encrypted);
    evaluator.relinearize_inplace(x5_encrypted, relin_keys);
    evaluator.rescale_to_next_inplace(x5_encrypted);
    Ciphertext x5_encrypted_coeff5;
    evaluator.mod_switch_to_inplace(plain_coeff5, last_parms_id1);
    evaluator.multiply_plain(x1_encrypted, plain_coeff5, x5_encrypted_coeff5);
    evaluator.rescale_to_next_inplace(x5_encrypted_coeff5);
    x5_encrypted.scale() = scale;
    x5_encrypted_coeff5.scale() = scale;
    parms_id_type last_parms_id5 = x5_encrypted.parms_id();
    evaluator.mod_switch_to_inplace(x5_encrypted_coeff5, last_parms_id5);
    evaluator.multiply_inplace(x5_encrypted, x5_encrypted_coeff5);
    evaluator.relinearize_inplace(x5_encrypted, relin_keys);
    evaluator.rescale_to_next_inplace(x5_encrypted);

    Ciphertext x3_encrypted;
    evaluator.square(x1_encrypted, x3_encrypted);
    evaluator.relinearize_inplace(x3_encrypted, relin_keys);
    evaluator.rescale_to_next_inplace(x3_encrypted);
    Ciphertext x1_encrypted_coeff3;
    evaluator.mod_switch_to_inplace(plain_coeff3, last_parms_id1);
    evaluator.multiply_plain(x1_encrypted, plain_coeff3, x1_encrypted_coeff3);
    evaluator.rescale_to_next_inplace(x1_encrypted_coeff3);
    evaluator.multiply_inplace(x3_encrypted, x1_encrypted_coeff3);
    evaluator.relinearize_inplace(x3_encrypted, relin_keys);
    evaluator.rescale_to_next_inplace(x3_encrypted);

    evaluator.mod_switch_to_inplace(plain_coeff1, last_parms_id1);
    evaluator.multiply_plain_inplace(x1_encrypted, plain_coeff1);
    evaluator.rescale_to_next_inplace(x1_encrypted);

    x9_encrypted.scale() = scale;
    x7_encrypted.scale() = scale;
    x5_encrypted.scale() = scale;
    x3_encrypted.scale() = scale;
    x1_encrypted.scale() = scale;

    // modulus switching.
    parms_id_type last_parms_id = x9_encrypted.parms_id();
    evaluator.mod_switch_to_inplace(x7_encrypted, last_parms_id);
    evaluator.mod_switch_to_inplace(x5_encrypted, last_parms_id);
    evaluator.mod_switch_to_inplace(x3_encrypted, last_parms_id);
    evaluator.mod_switch_to_inplace(x1_encrypted, last_parms_id);

    // add.
    Ciphertext encrypted_result_0;
    Ciphertext encrypted_result_1;
    Ciphertext encrypted_result_2;
    Ciphertext encrypted_result;
    evaluator.add(x3_encrypted, x1_encrypted, encrypted_result_0);
    evaluator.add(x5_encrypted, encrypted_result_0, encrypted_result_1);
    evaluator.add(x7_encrypted, encrypted_result_1, encrypted_result_2);
    evaluator.add(x9_encrypted, encrypted_result_2, encrypted_result);

    return encrypted_result;
}

Ciphertext iter(
    int times, int bit, double scale, RelinKeys relin_keys, Ciphertext x1_encrypted, Plaintext plain_coeff9,
    Plaintext plain_coeff7, Plaintext plain_coeff5, Plaintext plain_coeff3, Plaintext plain_coeff1)
{
    Ciphertext iter_result;
    Ciphertext iter_result1;
    Ciphertext iter_result2;
    Ciphertext iter_result3;
    Ciphertext iter_result4;
    switch (times)
    {
    case 0:
        iter_result = polynomial(
            bit, scale, relin_keys, x1_encrypted, plain_coeff9, plain_coeff7, plain_coeff5, plain_coeff3, plain_coeff1);
        return iter_result;
        break;

    case 1:
        iter_result = polynomial(
            bit, scale, relin_keys, x1_encrypted, plain_coeff9, plain_coeff7, plain_coeff5, plain_coeff3, plain_coeff1);
        iter_result1 = polynomial(
            bit, scale, relin_keys, iter_result, plain_coeff9, plain_coeff7, plain_coeff5, plain_coeff3, plain_coeff1);
        return iter_result1;
        break;

    case 2:
        iter_result = polynomial(
            bit, scale, relin_keys, x1_encrypted, plain_coeff9, plain_coeff7, plain_coeff5, plain_coeff3, plain_coeff1);
        iter_result1 = polynomial(
            bit, scale, relin_keys, iter_result, plain_coeff9, plain_coeff7, plain_coeff5, plain_coeff3, plain_coeff1);
        iter_result2 = polynomial(
            bit, scale, relin_keys, iter_result1, plain_coeff9, plain_coeff7, plain_coeff5, plain_coeff3, plain_coeff1);
        return iter_result2;
        break;

    case 3:
        iter_result = polynomial(
            bit, scale, relin_keys, x1_encrypted, plain_coeff9, plain_coeff7, plain_coeff5, plain_coeff3, plain_coeff1);
        iter_result1 = polynomial(
            bit, scale, relin_keys, iter_result, plain_coeff9, plain_coeff7, plain_coeff5, plain_coeff3, plain_coeff1);
        iter_result2 = polynomial(
            bit, scale, relin_keys, iter_result1, plain_coeff9, plain_coeff7, plain_coeff5, plain_coeff3, plain_coeff1);
        iter_result3 = polynomial(
            bit, scale, relin_keys, iter_result2, plain_coeff9, plain_coeff7, plain_coeff5, plain_coeff3, plain_coeff1);
        return iter_result3;
        break;

    case 4:
        iter_result = polynomial(
            bit, scale, relin_keys, x1_encrypted, plain_coeff9, plain_coeff7, plain_coeff5, plain_coeff3, plain_coeff1);
        iter_result1 = polynomial(
            bit, scale, relin_keys, iter_result, plain_coeff9, plain_coeff7, plain_coeff5, plain_coeff3, plain_coeff1);
        iter_result2 = polynomial(
            bit, scale, relin_keys, iter_result1, plain_coeff9, plain_coeff7, plain_coeff5, plain_coeff3, plain_coeff1);
        iter_result3 = polynomial(
            bit, scale, relin_keys, iter_result2, plain_coeff9, plain_coeff7, plain_coeff5, plain_coeff3, plain_coeff1);
        iter_result4 = polynomial(
            bit, scale, relin_keys, iter_result3, plain_coeff9, plain_coeff7, plain_coeff5, plain_coeff3, plain_coeff1);
        return iter_result4;
        break;

    default:
        break;
    }
}

vector<Ciphertext> MaxMin(
    int bit, double scale, RelinKeys relin_keys, Plaintext plain_coeff, Ciphertext y, Ciphertext x, Ciphertext sgnx)
{
    EncryptionParameters parms(scheme_type::ckks);
  
    cout<<"MaxMin"<<endl;
    // params.
    size_t poly_modulus_degree = 16384 * 2;
    parms.set_poly_modulus_degree(poly_modulus_degree);
    parms.set_coeff_modulus(
        CoeffModulus::Create(poly_modulus_degree, { 60,  bit, bit, bit, bit, bit, bit, bit, bit, bit, bit, bit,
                                                    bit, bit, bit, bit, bit, bit, bit, bit, bit, bit, 60 }));
    SEALContext context(parms);
    //print_parameters(context);
    Evaluator evaluator(context);

    // calculate 0.5*x*sgn(x)
    Ciphertext x_encrypted_coeff;
    parms_id_type last_parms_id_plain = x.parms_id();
    evaluator.mod_switch_to_inplace(plain_coeff, last_parms_id_plain);
    evaluator.multiply_plain(x, plain_coeff, x_encrypted_coeff);
    evaluator.rescale_to_next_inplace(x_encrypted_coeff);

    sgnx.scale() = scale;
    x_encrypted_coeff.scale() = scale;
    parms_id_type last_parms_id_sgn = sgnx.parms_id();
    evaluator.mod_switch_to_inplace(x_encrypted_coeff, last_parms_id_sgn);

    evaluator.multiply_inplace(sgnx, x_encrypted_coeff);
    evaluator.relinearize_inplace(sgnx, relin_keys);
    evaluator.rescale_to_next_inplace(sgnx);

    // calculate 0.5*y
    Ciphertext y_result;
    evaluator.mod_switch_to_inplace(y, last_parms_id_plain);
    evaluator.multiply_plain(y, plain_coeff, y_result);
    evaluator.rescale_to_next_inplace(y_result);

    // calculate max, min
    sgnx.scale() = scale;
    y_result.scale() = scale;
    parms_id_type last_parms_id_result = sgnx.parms_id();
    evaluator.mod_switch_to_inplace(y_result, last_parms_id_result);

    Ciphertext max_result;
    evaluator.add(y_result, sgnx, max_result);
    Ciphertext min_result;
    evaluator.sub(y_result, sgnx, min_result);

    // Decrypt,decode
    vector<Ciphertext> result;
    result.emplace_back(max_result);
    result.emplace_back(min_result);

    return result;
}

vector<Ciphertext> MaxMinOfTwoCipher(int iter_time,
    int bit, double scale, RelinKeys relin_keys, Plaintext plain_coeff, Ciphertext x1, Ciphertext x2,
    Plaintext plain_coeff9, Plaintext plain_coeff7, Plaintext plain_coeff5, Plaintext plain_coeff3,
    Plaintext plain_coeff1)
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
    Ciphertext iterResult;
    iterResult = iter(
        iter_time, bit, scale, relin_keys, x1_encrypted, plain_coeff9, plain_coeff7, plain_coeff5, plain_coeff3, plain_coeff1);

    // calculate max,min
    vector<Ciphertext> cipher_result;
    cipher_result = MaxMin(bit, scale, relin_keys, plain_coeff, sum, x1_encrypted, iterResult);

    return cipher_result;
}

vector<double> sort_2Cipher(int iter_time,
    int bit, vector<double> x, double coeff9, double coeff7, double coeff5, double coeff3, double coeff1)
{
    EncryptionParameters parms(scheme_type::ckks);

    // params.
    cout<<"sort_2Cipher"<<endl;
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

    Plaintext plain_coeff9, plain_coeff7, plain_coeff5, plain_coeff3, plain_coeff1, plain_coeff0;
    encoder.encode(coeff9, scale, plain_coeff9);
    encoder.encode(coeff7, scale, plain_coeff7);
    encoder.encode(coeff5, scale, plain_coeff5);
    encoder.encode(coeff3, scale, plain_coeff3);
    encoder.encode(coeff1, scale, plain_coeff1);
    Plaintext plain_coeff;
    encoder.encode(0.5, scale, plain_coeff);

    // server
    // calculate max,min of two ciphertext
    vector<Ciphertext> cipher_result;
    cipher_result = MaxMinOfTwoCipher(iter_time,
        bit, scale, relin_keys, plain_coeff, inputc1, inputc2, plain_coeff9, plain_coeff7, plain_coeff5, plain_coeff3,
        plain_coeff1);

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

void sort_TwoCipher_f()
{
    double a;
    double b; 
    
    double coeff9 = 0.2734375;
    double coeff7 = -1.40625;
    double coeff5 = 2.953125;
    double coeff3 = -3.28125;
    double coeff1 = 2.4609375;
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
