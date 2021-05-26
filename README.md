# knowledge-share

# Introduction
seal is code of a fully homomorphic encryption scheme: ckks.

# What I am doing

I plan to make some expansions on it. (SEAL/native/examples/8~14)

Now I have done three things:
1.Linear regression;(8)
2.Sort two ciphertexts, decrypt only at final result.(11)
3.ReLU(13)

And three try:
1.Sort two ciphertexts, iterate four times. Decrypting each time makes it no sense.(9) 
2.Max and Min, base on (9).(10)
3.Sort three ciphertexts, it is limited by the depth.(12)

I am writing a simple neuron network(14) used ReLU under ckks. To be continued……

# Configuration

MacOS: VSCode, CMake

Commandline: 
1.cmake -S . -B build
2.cmake --build build
3.sudo cmake --install build
