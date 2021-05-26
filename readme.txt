# Introduction
seal is code of a fully homomorphic encryption scheme: ckks.

# What I am doing

I plan to make some expansions on it. (SEAL/native/examples/10,11,12,14,15,20,21,22,30,31,32,40,41,42,50,51,52)

Now I have done five things:
1.three approximate polynomial to sort.(10,11,12). done.
2.three approximate polynomial to relu.(20,21,22). done.
3.train,test.(14,15). done.
4.test cnn with relu.(30,31,32). done.
5.knn with sort.(40,41,42). done.
6.test seal.

I am writing logistic regression.

# Configuration

MacOS: VSCode, CMake

Commandline: 
1.cmake -S . -B build
2.cmake --build build
3.sudo cmake --install build