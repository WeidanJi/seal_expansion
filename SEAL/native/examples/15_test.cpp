#include <iostream>
#include <stdio.h>
#include "examples.h"

using namespace std;
using namespace seal;

double Test(double a,double b,double w0_0,double w0_1,double b0,double w1,double b1,double hide)
{
    int bit = 40;
    EncryptionParameters parms(scheme_type::ckks);

    // params.
    size_t poly_modulus_degree = 16384;
    parms.set_poly_modulus_degree(poly_modulus_degree);
    parms.set_coeff_modulus(
        CoeffModulus::Create(poly_modulus_degree, { 60, bit, bit, bit, bit, bit, bit, bit, 60 }));
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

    //(aw0+bw0+b0)*w1
    last_parms_id = aw00_cipher.parms_id();
    evaluator.mod_switch_to_inplace(w1_cipher, last_parms_id);
    aw00_cipher.scale()=scale;
    w1_cipher.scale()=scale;
    Ciphertext abw1_cipher;
    evaluator.multiply(aw00_cipher, w1_cipher, abw1_cipher);
    evaluator.relinearize_inplace(abw1_cipher, relin_keys);
    evaluator.rescale_to_next_inplace(abw1_cipher);

    //(aw0+bw0+b0)*w1*hide
    last_parms_id = abw1_cipher.parms_id();
    evaluator.mod_switch_to_inplace(hide_cipher, last_parms_id);
    abw1_cipher.scale()=scale;
    hide_cipher.scale()=scale;
    Ciphertext abw1_hide_cipher;
    evaluator.multiply(abw1_cipher, hide_cipher, abw1_hide_cipher);
    evaluator.relinearize_inplace(abw1_hide_cipher, relin_keys);
    evaluator.rescale_to_next_inplace(abw1_hide_cipher);

    //(aw0+bw0+b0)*w1*hide+b1
    abw1_hide_cipher.scale()=scale;
    b1_cipher.scale()=scale;
    last_parms_id = abw1_hide_cipher.parms_id();
    evaluator.mod_switch_to_inplace(b1_cipher, last_parms_id);
    evaluator.add_inplace(abw1_hide_cipher, b1_cipher);

    //2*[(aw0+bw0+b0)*w1*hide+b1]
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

void test()
{
    double a=0.9;
    double b=0.7;
    double w0_0=0.36039;
    double w0_1=0.36043;
    double b0=-0.02264;
    double w1=0.03468;
    double b1=0.03141;
    double hide=40;

    double res;
    res=Test(a,b,w0_0,w0_1,b0,w1,b1,hide);
    cout<<"predict result: "<<res<<endl;

    double act_res;
    act_res=2*((w0_0*a+w0_1*b+b0)*w1*hide+b1);
    cout<<"actual result: "<<act_res<<endl;

    double ab_res;
    ab_res=a+b;
    cout<<"a+b: "<<ab_res<<endl;

}