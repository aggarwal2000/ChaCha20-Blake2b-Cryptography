#pragma once

std::vector<uint8_t> Chacha20Decrypt(const std::vector<uint8_t> & cipher_text_message, const std::array<uint8_t, 32> & key,
 const std::array< uint8_t, 12> & nonce, const uint32_t counter);

std::vector<uint8_t> Chacha20Encrypt(const std::vector<uint8_t> & plain_text_message, const std::array<uint8_t, 32> & key, 
const std::array< uint8_t, 12> & nonce, const uint32_t counter);
