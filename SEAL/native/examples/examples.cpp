// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT license.

#include "examples.h"

using namespace std;
using namespace seal;

int main()
{
    cout << "Microsoft SEAL version: " << SEAL_VERSION << endl;
    while (true)
    {
        cout << "+------------------------------------------------------------+" << endl;
        cout << "| The following examples should be executed while reading    |" << endl;
        cout << "| comments in associated files in native/examples/.          |" << endl;
        cout << "+------------------------------------------------------------+" << endl;
        cout << "| Examples                   | Source Files                  |" << endl;
        cout << "+----------------------------+-------------------------------+" << endl;
        cout << "| 1. BFV Basics              | 1_bfv_basics.cpp              |" << endl;
        cout << "| 2. Encoders                | 2_encoders.cpp                |" << endl;
        cout << "| 3. Levels                  | 3_levels.cpp                  |" << endl;
        cout << "| 4. CKKS Basics             | 4_ckks_basics.cpp             |" << endl;
        cout << "| 5. Rotation                | 5_rotation.cpp                |" << endl;
        cout << "| 6. Serialization           | 6_serialization.cpp           |" << endl;
        cout << "| 7. Performance Test        | 7_performance.cpp             |" << endl;
        
        cout << "| 10.Sort Two Cipher taylor  | 10_sort_TwoCipher_taylor.cpp  |" << endl;
        cout << "| 11.Sort Two Cipher f       | 11_sort_TwoCipher_f.cpp       |" << endl;
        cout << "| 12.Sort Two Cipher gf      | 12_sort_TwoCipher_gf.cpp      |" << endl;
        cout << "| 13.Sort Two Cipher LS      | 13_sort_TwoCipher_LS.cpp      |" << endl;

        cout << "| 20.Relu taylor             | 20_relu_taylor.cpp            |" << endl;
        cout << "| 21.Relu f                  | 21_relu_f.cpp                 |" << endl;
        cout << "| 22.Relu gf                 | 22_relu_gf.cpp                |" << endl;
        cout << "| 23.Relu LS                 | 23_relu_LS.cpp                |" << endl;

        cout << "| 14.Train                   | 14_train.cpp                  |" << endl;
        cout << "| 15.Test                    | 15_test.cpp                   |" << endl;
        cout << "| 30.Test ReLU taylor        | 30_test_relu_taylor.cpp       |" << endl;
        cout << "| 31.Test ReLU f             | 31_test_relu_f.cpp            |" << endl;
        cout << "| 32.Test ReLU gf            | 32_test_relu_gf.cpp           |" << endl;
        cout << "| 33.Test ReLU LS            | 33_test_relu_LS.cpp           |" << endl;

        cout << "| 40.KNN taylor              | 40_knn_taylor.cpp             |" << endl;
        cout << "| 41.KNN f                   | 41_knn_f.cpp                  |" << endl;
        cout << "| 42.KNN gf                  | 42_knn_gf.cpp                 |" << endl;

        cout << "| 50.Run float add 1         | 50_run_floatadd1.cpp          |" << endl;
        cout << "| 51.Run float sub 1         | 51_run_floatsub1.cpp          |" << endl;
        cout << "| 52.Run float mul 1         | 52_run_floatmul1.cpp          |" << endl;
        
        cout << "| 60.Run uint32 add 5        | 60_run_uint32add5.cpp         |" << endl;
        cout << "| 61.Run uint32 sub 5        | 61_run_uint32sub5.cpp         |" << endl;
        cout << "| 62.Run uint32 mul 5        | 62_run_uint32mul5.cpp         |" << endl;
         
        cout << "| 70.Run uint64 add 5        | 70_run_uint64add5.cpp         |" << endl;
        cout << "| 71.Run uint64 sub 5        | 71_run_uint64sub5.cpp         |" << endl;
        cout << "| 72.Run uint64 mul 5        | 72_run_uint64mul5.cpp         |" << endl;
         
        cout << "+----------------------------+-------------------------------+" << endl;

        /*
        Print how much memory we have allocated from the current memory pool.
        By default the memory pool will be a static global pool and the
        MemoryManager class can be used to change it. Most users should have
        little or no reason to touch the memory allocation system.
        */
        size_t megabytes = MemoryManager::GetPool().alloc_byte_count() >> 20;
        cout << "[" << setw(7) << right << megabytes << " MB] "
             << "Total allocation from the memory pool" << endl;

        int selection = 0;
        bool valid = true;
        do
        {
            cout << endl << "> Run example (1 ~ 100) or exit (0): ";
            if (!(cin >> selection))
            {
                valid = false;
            }
            else if (selection < 0 || selection > 100)
            {
                valid = false;
            }
            else
            {
                valid = true;
            }
            if (!valid)
            {
                cout << "  [Beep~~] valid option: type 0 ~ 100" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        } while (!valid);

        switch (selection)
        {
        case 1:
            example_bfv_basics();
            break;

        case 2:
            example_encoders();
            break;

        case 3:
            example_levels();
            break;

        case 4:
            example_ckks_basics();
            break;

        case 5:
            example_rotation();
            break;

        case 6:
            example_serialization();
            break;

        case 7:
            example_performance_test();
            break;

        //sort
        case 10:
            sort_TwoCipher_taylor();
            break;
        case 11:
            sort_TwoCipher_f();
            break;
        case 12:
            sort_TwoCipher_gf();
            break;
        case 13:
            sort_TwoCipher_LS();
            break;

        //relu
        case 20:
            relu_taylor();
            break;
        case 21:
            relu_f();
            break;
        case 22:
            relu_gf();
            break;
        case 23:
            relu_LS();
            break;
        
        //train
        case 14:
            train();
            break;
        
        //test
        case 15:
            test();
            break;
        case 30:
            test_relu_taylor();
            break;
        case 31:
            test_relu_f();
            break;
        case 32:
            test_relu_gf();
            break;
        case 33:
            test_relu_LS();
            break;

        //knn
        case 40:
            knn_taylor();
            break;
        case 41:
            knn_f();
            break;
        case 42:
            knn_gf();
            break;

        case 50:
            run_floatadd1();
            break;
        
        case 51:
            run_floatsub1();
            break;
   
        case 52:
            run_floatmul1();
            break;
            
        case 0:
            return 0;
        }
    }

    return 0;
}
