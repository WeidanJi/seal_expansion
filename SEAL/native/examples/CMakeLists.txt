# Copyright (c) Microsoft Corporation. All rights reserved.
# Licensed under the MIT license.

cmake_minimum_required(VERSION 3.12)

project(SEALExamples VERSION 3.6.4 LANGUAGES CXX)

# If not called from root CMakeLists.txt
if(NOT DEFINED SEAL_BUILD_EXAMPLES)
    set(SEAL_BUILD_EXAMPLES ON)

    # Import Microsoft SEAL
    find_package(SEAL 3.6.4 EXACT REQUIRED)

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
endif()

if(SEAL_BUILD_EXAMPLES)
    add_executable(sealexamples)

    target_sources(sealexamples
        PRIVATE
            ${CMAKE_CURRENT_LIST_DIR}/examples.cpp
            ${CMAKE_CURRENT_LIST_DIR}/1_bfv_basics.cpp
            ${CMAKE_CURRENT_LIST_DIR}/2_encoders.cpp
            ${CMAKE_CURRENT_LIST_DIR}/3_levels.cpp
            ${CMAKE_CURRENT_LIST_DIR}/4_ckks_basics.cpp
            ${CMAKE_CURRENT_LIST_DIR}/5_rotation.cpp
            ${CMAKE_CURRENT_LIST_DIR}/6_serialization.cpp
            ${CMAKE_CURRENT_LIST_DIR}/7_performance.cpp
            ${CMAKE_CURRENT_LIST_DIR}/10_sort_TwoCipher_taylor.cpp
            ${CMAKE_CURRENT_LIST_DIR}/11_sort_TwoCipher_f.cpp
            ${CMAKE_CURRENT_LIST_DIR}/12_sort_TwoCipher_gf.cpp
            ${CMAKE_CURRENT_LIST_DIR}/13_sort_TwoCipher_LS.cpp
            ${CMAKE_CURRENT_LIST_DIR}/20_relu_taylor.cpp
            ${CMAKE_CURRENT_LIST_DIR}/21_relu_f.cpp
            ${CMAKE_CURRENT_LIST_DIR}/22_relu_gf.cpp
            ${CMAKE_CURRENT_LIST_DIR}/23_relu_LS.cpp
            ${CMAKE_CURRENT_LIST_DIR}/14_train.cpp
            ${CMAKE_CURRENT_LIST_DIR}/15_test.cpp
            ${CMAKE_CURRENT_LIST_DIR}/30_test_relu_taylor.cpp
            ${CMAKE_CURRENT_LIST_DIR}/31_test_relu_f.cpp
            ${CMAKE_CURRENT_LIST_DIR}/32_test_relu_gf.cpp
            ${CMAKE_CURRENT_LIST_DIR}/33_test_relu_LS.cpp
            ${CMAKE_CURRENT_LIST_DIR}/40_knn_taylor.cpp
            ${CMAKE_CURRENT_LIST_DIR}/41_knn_f.cpp
            ${CMAKE_CURRENT_LIST_DIR}/42_knn_gf.cpp
            ${CMAKE_CURRENT_LIST_DIR}/50_run_floatadd1.cpp
            ${CMAKE_CURRENT_LIST_DIR}/51_run_floatsub1.cpp
            ${CMAKE_CURRENT_LIST_DIR}/52_run_floatmul1.cpp
    )

    if(TARGET SEAL::seal)
        target_link_libraries(sealexamples PRIVATE SEAL::seal)
    elseif(TARGET SEAL::seal_shared)
        target_link_libraries(sealexamples PRIVATE SEAL::seal_shared)
    else()
        message(FATAL_ERROR "Cannot find target SEAL::seal or SEAL::seal_shared")
    endif()
endif()
