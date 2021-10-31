#bin/bash!

g++ -std=c++17 Blake2b.cpp ChaCha20Cipher.cpp Authentication_Encryption.cpp IO_utility.cpp main.cpp -o AE

./AE $1 $2