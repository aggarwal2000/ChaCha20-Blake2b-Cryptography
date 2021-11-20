#pragma once

void ReadInputFiles(std::vector<uint8_t>& plain_text_message, std::array< uint8_t, 32> & chacha20_key, std::array< uint8_t, 12>& chacha20_nonce, 
uint32_t & chacha20_counter, std::vector<uint8_t>& hmac_key, size_t & hash_num_bytes);

void ReadInputFiles(std::vector<uint8_t>& plain_text_message, std::array< uint8_t, 32> & chacha20_key, std::array< uint8_t, 12> & chacha20_nonce, 
uint32_t & chacha20_counter);

void ReadInputFiles(std::vector<uint8_t>& text_message,std::vector<uint8_t>&  blake_key, size_t & digest_num_bytes);

void write(const std::vector<uint8_t>& bytes, const bool is_str, const std::string & fname);

void write(const std::string & to_write , const std::string & fname);

void DeleteFilesInDir(const std::string & dir);