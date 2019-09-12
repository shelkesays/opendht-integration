// #pragma once;

#include <opendht/crypto.h>


bool test_encrypt_message(std::vector<uint8_t> message) {
    auto key = dht::crypto::PrivateKey::generate();
    auto public_key = key.getPublicKey();

    // Encrypt
    std::vector<uint8_t> encrypted = public_key.encrypt(message);
    // std::cout << encrypted.capacity() << std::endl;
    
    // Decrypt
    std::vector<uint8_t> decrypted = key.decrypt(encrypted);
    // std::cout << decrypted.capacity() << std::endl;

    return (message == decrypted);
}


int main() {
    std::vector<uint8_t> message;

    for (int i = 1; i <= 5; i++) {
        message.push_back(i);
    }
    
    std::cout << test_encrypt_message(message) << std::endl;

    return 0;
}