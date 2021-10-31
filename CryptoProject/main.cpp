#include<iostream>
#include<vector>
#include<array>
#include<string>

#include"IO_utility.h"
#include"ChaCha20Cipher.h"
#include"Blake2b.h"


int main()
{	

	std::vector<uint8_t> plain_text_message;
	std::array< uint8_t, 32> chacha_key;
	std::array< uint8_t, 12> chacha_nonce; 
	uint32_t chacha_counter;
	std::vector<uint8_t> blake_key;
	size_t digest_num_bytes;

	ReadInputFiles(plain_text_message, chacha_key, chacha_nonce, chacha_counter, blake_key, digest_num_bytes);

	std::vector<uint8_t> cipher_text_message = Chacha20Encrypt(plain_text_message, chacha_key, chacha_nonce, chacha_counter);
	std::vector<uint8_t> decrypted_text_message = Chacha20Decrypt(cipher_text_message, chacha_key, chacha_nonce, chacha_counter);

	
	// printf("\n Plain Text Message: \n");

	// for(int i = 0; i < plain_text_message.size(); i++)
	// {
	// 	printf("\n i: %d, %x , %c ",i, plain_text_message[i] , plain_text_message[i]);
	// }

	// printf("\n Cipher (Encrypted) Text Message: \n");

	// for(int i = 0; i < cipher_text_message.size(); i++)
	// {
	// 	printf("\n i: %d, %x , %c ",i, cipher_text_message[i] , cipher_text_message[i]);
	// }

	// printf("\n Decrypted Text Message: \n");

	// for(int i = 0; i < decrypted_text_message.size(); i++)
	// {
	// 	printf("\n i: %d, %x , %c ",i, decrypted_text_message[i] , decrypted_text_message[i]);
	// }

	// std::cout << "chacha key: " << std::endl;
	// for(int i = 0; i < chacha_key.size(); i++)
	// {
	// 	printf("\n i:%d, key byte: %x ", i, chacha_key[i]);
	// }
	// printf("\n");


	// std::cout << "chacha nonce: " << std::endl;
	// for(int i = 0; i < chacha_nonce.size(); i++)
	// {
	// 	printf("\n i:%d, key byte: %x ", i, chacha_nonce[i]);
	// }
	// printf("\n");
	

	// printf("\n digest num bytes: %d ", digest_num_bytes);
	// printf("\n blake key size: %d ", blake_key.size());
	
	std::vector<uint8_t> hash_tag_bytes_alice =  Blake2b(blake_key , cipher_text_message, digest_num_bytes);
	std::vector<uint8_t> hash_tag_bytes_bob =  Blake2b(blake_key , cipher_text_message, digest_num_bytes);

	
	//std::cout << "hash tag bytes: " << std::endl;
	// for(int i = 0; i < hash_tag_bytes.size(); i++)
	// {
	// 	printf("\n i: %d , hash tag byte: %x ", i, hash_tag_bytes[i]);
	// }

	write(cipher_text_message, true, "OutputFiles/encrypted_text.txt");
	write(decrypted_text_message, true, "OutputFiles/decrypted_text.txt");
	write(hash_tag_bytes_alice, false, "OutputFiles/hash_alice.txt");
	write(hash_tag_bytes_bob, false, "OutputFiles/hash_bob.txt");

	
}
