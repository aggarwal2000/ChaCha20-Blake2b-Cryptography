#pragma once

std::vector<uint8_t> Blake2b(const std::vector<uint8_t> & hash_key , const std::vector<uint8_t> & message,
 const size_t hash_num_bytes);