cmake_minimum_required(VERSION 2.8.9)
project (vigenere_cipher)

set (VIGENERE_CIPHER_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}")

set(SOURCE_FILES
    vigenere_cipher.cpp
)

add_executable(vigenere_cipher ${SOURCE_FILES})