#include<vector>
#include<array>
#include<vector>
#include<cassert>
#include<random>
#include<iostream>

#include"IO_utility.h"
#include"ChaCha20Cipher.h"
#include"Blake2b.h"


namespace{


bool IsDigestSame(const std::vector<uint8_t> & hash_tag_bytes_alice, const std::vector<uint8_t> & hash_tag_bytes_bob )
{
    for(int i = 0; i < hash_tag_bytes_alice.size(); i++)
    {   
        if(hash_tag_bytes_alice[i] != hash_tag_bytes_bob[i])
        {
            return false;
        }
    }
    return true;
}


}//unnamed namespace

void SoleEncryption(bool forged_by_adversary)
{   


    DeleteFilesInDir("OutputFiles/");
    std::cout << "\n Sole Encryption\n";

    std::vector<uint8_t> plain_text_message;
	std::array< uint8_t, 32> chacha_key;
	std::array< uint8_t, 12> chacha_nonce; 
	uint32_t chacha_counter;

	ReadInputFiles(plain_text_message, chacha_key, chacha_nonce, chacha_counter);

	std::vector<uint8_t> cipher_text_message_generated_by_Alice = Chacha20Encrypt(plain_text_message, chacha_key, chacha_nonce, chacha_counter);

    std::vector<uint8_t> cipher_text_message_reaching_Bob = cipher_text_message_generated_by_Alice;
    if(forged_by_adversary)
    {   
        assert(cipher_text_message_reaching_Bob.size() >= 0);
        srand(1);
        const size_t index = rand()%cipher_text_message_reaching_Bob.size();
        cipher_text_message_reaching_Bob[index] = ~ cipher_text_message_reaching_Bob[index];
    }
  
    write(cipher_text_message_generated_by_Alice, false, "OutputFiles/cipher_text_generated_by_Alice.txt");
    write(cipher_text_message_reaching_Bob, false, "OutputFiles/cipher_text_reaching_Bob.txt");

    std::vector<uint8_t> decrypted_text_message = Chacha20Decrypt(cipher_text_message_reaching_Bob, chacha_key, chacha_nonce, chacha_counter);
    write(decrypted_text_message, true, "OutputFiles/decrypted_text.txt");

}


void AuthenticatedEncryption(bool forged_by_adversary)
{   
    DeleteFilesInDir("OutputFiles/");
    std::cout << "\n Authenticated Encryption\n";

    std::vector<uint8_t> plain_text_message;
	std::array< uint8_t, 32> chacha_key;
	std::array< uint8_t, 12> chacha_nonce; 
	uint32_t chacha_counter;
	std::vector<uint8_t> blake_key;
	size_t digest_num_bytes;

	ReadInputFiles(plain_text_message, chacha_key, chacha_nonce, chacha_counter, blake_key, digest_num_bytes);

	std::vector<uint8_t> cipher_text_message_generated_by_Alice = Chacha20Encrypt(plain_text_message, chacha_key, chacha_nonce, chacha_counter);
    std::vector<uint8_t> hash_tag_bytes_alice =  Blake2b(blake_key , cipher_text_message_generated_by_Alice, digest_num_bytes);

    std::vector<uint8_t> cipher_text_message_reaching_Bob = cipher_text_message_generated_by_Alice;
    if(forged_by_adversary)
    {   
        assert(cipher_text_message_reaching_Bob.size() >= 0);
        srand(1);
        const size_t index = rand()%cipher_text_message_reaching_Bob.size();
        cipher_text_message_reaching_Bob[index] = ~ cipher_text_message_reaching_Bob[index];
    }
  
    std::vector<uint8_t> hash_tag_bytes_bob =  Blake2b(blake_key , cipher_text_message_reaching_Bob, digest_num_bytes);

    write(cipher_text_message_generated_by_Alice, false, "OutputFiles/cipher_text_generated_by_Alice.txt");
    write(cipher_text_message_reaching_Bob, false, "OutputFiles/cipher_text_reaching_Bob.txt");
	write(hash_tag_bytes_alice, false, "OutputFiles/hash_alice.txt");
	write(hash_tag_bytes_bob, false, "OutputFiles/hash_bob.txt");

    if(IsDigestSame(hash_tag_bytes_alice, hash_tag_bytes_bob))
    {   
        write("Verified over hash tag", "OutputFiles/tag_verification_result.txt");
        std::vector<uint8_t> decrypted_text_message = Chacha20Decrypt(cipher_text_message_reaching_Bob, chacha_key, chacha_nonce, chacha_counter);
        write(decrypted_text_message, true, "OutputFiles/decrypted_text.txt");
    }
    else
    {   
        write("Tag mismatch, message has been forged by adversary", "OutputFiles/tag_verification_result.txt");
        write("Message has been forged by the adversary(found by HMAC- hash tag verification), no use of decryption. ", "OutputFiles/decrypted_text.txt");
    }

}