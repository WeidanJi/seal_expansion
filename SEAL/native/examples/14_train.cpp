#include <iostream>
#include <stdio.h>
#include "examples.h"
#include "relu.h"
#include "sort_Cipher.h"

using namespace std;
using namespace seal;

vector<double> Train(int bit)
{
    /*
    // input
    vector<double> x1{ 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0 };
    vector<double> y1{ 1.2, 2.5, 3.7, 4.6, 5.9, 6.5, 7.8 };

    EncryptionParameters parms(scheme_type::ckks);

    // params.
    size_t poly_modulus_degree = 16384 * 2;
    parms.set_poly_modulus_degree(poly_modulus_degree);
    parms.set_coeff_modulus(
        CoeffModulus::Create(poly_modulus_degree, { 60,  bit, bit, bit, bit, bit, bit, bit, bit, bit, bit, bit,
                                                    bit, bit, bit, bit, bit, bit, bit, bit, bit, bit, 60 }));
    SEALContext context(parms);
    print_parameters(context);
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
    vector<Plaintext> pts_x;
    for (auto val : x1)
    {
        Plaintext p_x;

        encoder.encode(val, scale, p_x);
        pts_x.emplace_back(move(p_x));
    }
    vector<Plaintext> pts_y;
    for (auto val : y1)
    {
        Plaintext p_y;

        encoder.encode(val, scale, p_y);
        pts_y.emplace_back(move(p_y));
    }
    vector<Ciphertext> cts_x;
    for (const auto &p_x : pts_x)
    {
        Ciphertext c_x;
        encryptor.encrypt(p_x, c_x);
        cts_x.emplace_back(move(c_x));
    }
    vector<Ciphertext> cts_y;
    for (const auto &p_y : pts_y)
    {
        Ciphertext c_y;
        encryptor.encrypt(p_y, c_y);
        cts_y.emplace_back(move(c_y));
    }

    // y=wx+b
    Plaintext plain_w, plain_b;
    encoder.encode(1.0, scale, plain_w);
    encoder.encode(1.0, scale, plain_b);
    Ciphertext cipher_w, cipher_b;
    encryptor.encrypt(plain_w, cipher_w);
    encryptor.encrypt(plain_b, cipher_b);
    Plaintext plain_size;
    encoder.encode(1.0 / x1.size(), scale, plain_size);
    Ciphertext cipher_size;
    encryptor.encrypt(plain_size, cipher_size);

    // server

    int iteration = 100;
    double learning_rate = 0.001;
    Plaintext plain_lrate;
    encoder.encode(learning_rate, scale, plain_lrate);
    Ciphertext cipher_lrate;
    encryptor.encrypt(plain_lrate, cipher_lrate);

    // calculate predictions=wx+b
    vector<Ciphertext> predictions;
    for (int i = 0; i < cts_x.size(); i++)
    {
        Ciphertext prediction;
        evaluator.multiply(cipher_w, cts_x[i], prediction);
        evaluator.relinearize_inplace(prediction, relin_keys);
        evaluator.rescale_to_next_inplace(prediction);
        cout << "ssss" << endl;
        parms_id_type last_parms_id = prediction.parms_id();
        evaluator.mod_switch_to_inplace(cipher_b, last_parms_id);
        prediction.scale() = scale;
        cipher_b.scale() = scale;
        evaluator.add_inplace(prediction, cipher_b);

        predictions.emplace_back(prediction);
    }

    // calculate diff
    vector<Ciphertext> diffs;
    for (int i = 0; i < cts_y.size(); i++)
    {
        Ciphertext diff;

        cts_y[i].scale() = scale;
        predictions[i].scale() = scale;
        cout << "aaaaaa" << endl;
        parms_id_type last_parms_id = predictions[i].parms_id();
        evaluator.mod_switch_to_inplace(cts_y[i], last_parms_id);

        evaluator.sub(predictions[i], cts_y[i], diff);
        diffs.emplace_back(diff);
    }

    // calculate derivative of w
    vector<Ciphertext> w_derivatives;
    for (int i = 0; i < cts_x.size(); i++)
    {
        Ciphertext w_derivative;
        cout << "ddddd" << endl;
        parms_id_type last_parms_id = predictions[i].parms_id();
        evaluator.mod_switch_to_inplace(cts_x[i], last_parms_id);
        evaluator.multiply(predictions[i], cts_x[i], w_derivative);
        evaluator.relinearize_inplace(w_derivative, relin_keys);
        evaluator.rescale_to_next_inplace(w_derivative);

        w_derivatives.emplace_back(w_derivative);
    }

    Ciphertext w_diff_x;
    evaluator.add_many(w_derivatives, w_diff_x);

    Ciphertext w_diff_x_m;
    w_diff_x.scale() = scale;
    cipher_size.scale() = scale;
    cout << "ffffff" << endl;
    parms_id_type last_parms_id = w_diff_x.parms_id();
    evaluator.mod_switch_to_inplace(cipher_size, last_parms_id);
    evaluator.multiply(w_diff_x, cipher_size, w_diff_x_m);
    evaluator.relinearize_inplace(w_diff_x_m, relin_keys);
    evaluator.rescale_to_next_inplace(w_diff_x_m);

    Ciphertext w_diff_x_m_lrate;
    w_diff_x_m.scale() = scale;
    cipher_lrate.scale() = scale;
    cout << "ggggg" << endl;
    last_parms_id = w_diff_x_m.parms_id();
    evaluator.mod_switch_to_inplace(cipher_lrate, last_parms_id);
    evaluator.multiply(w_diff_x_m, cipher_lrate, w_diff_x_m_lrate);
    evaluator.relinearize_inplace(w_diff_x_m_lrate, relin_keys);
    evaluator.rescale_to_next_inplace(w_diff_x_m_lrate);

    // calculate derivative of b
    vector<Ciphertext> b_derivatives;
    for (int i = 0; i < diffs.size(); i++)
    {
        b_derivatives.emplace_back(diffs[i]);
    }

    Ciphertext b_diff;
    evaluator.add_many(b_derivatives, b_diff);

    Ciphertext b_diff_m;
    b_diff.scale() = scale;
    cipher_size.scale() = scale;
    cout << "hhhhhh" << endl;
    last_parms_id = cipher_size.parms_id();
    evaluator.mod_switch_to_inplace(b_diff, last_parms_id);
    evaluator.multiply(b_diff, cipher_size, b_diff_m);
    evaluator.relinearize_inplace(b_diff_m, relin_keys);
    evaluator.rescale_to_next_inplace(b_diff_m);

    Ciphertext b_diff_m_lrate;
    b_diff_m.scale() = scale;
    cipher_lrate.scale() = scale;
    cout << "jjjjj" << endl;
    last_parms_id = b_diff_m.parms_id();
    evaluator.mod_switch_to_inplace(cipher_lrate, last_parms_id);
    evaluator.multiply(b_diff_m, cipher_lrate, b_diff_m_lrate);
    evaluator.relinearize_inplace(b_diff_m_lrate, relin_keys);
    evaluator.rescale_to_next_inplace(b_diff_m_lrate);
    cout << "kkkkk" << endl;
    // refresh w,b
    w_diff_x_m_lrate.scale() = scale;
    cipher_w.scale() = scale;
    last_parms_id = w_diff_x_m_lrate.parms_id();
    evaluator.mod_switch_to_inplace(cipher_w, last_parms_id);
    evaluator.sub_inplace(cipher_w, w_diff_x_m_lrate);

    b_diff_m_lrate.scale() = scale;
    cipher_b.scale() = scale;
    last_parms_id = b_diff_m_lrate.parms_id();
    evaluator.mod_switch_to_inplace(cipher_b, last_parms_id);
    evaluator.sub_inplace(cipher_b, b_diff_m_lrate);
    cout << "lllll" << endl;
    // local
    // decrypt,decode
    Plaintext w_plainresult, b_plainresult;
    decryptor.decrypt(cipher_w, w_plainresult);
    decryptor.decrypt(cipher_b, b_plainresult);
    vector<double> w_result, b_result;
    encoder.decode(w_plainresult, w_result);
    encoder.decode(b_plainresult, b_result);
    cout << "zzzzz" << endl;
    vector<double> result;
    result.push_back(w_result[0]);
    result.push_back(b_result[0]);

    return result;
    */
}

void train()
{
    /*
    vector<double> w_b;
    double w, b;
    w_b = Train(35);
    w = w_b[0];
    b = w_b[1];
    cout << "w: " << w << endl;
    cout << "b: " << b << endl;
    */
}