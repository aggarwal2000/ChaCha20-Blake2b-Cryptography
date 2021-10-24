#include<iostream>
#include<vector>
#include<array>
#include<string>


std::vector<uint8_t> Chacha20Decrypt(const std::vector<uint8_t> & cipher_text_message, const std::array<uint8_t, 32> & key, const std::array< uint8_t, 12> & nonce, const uint32_t counter);
std::vector<uint8_t> Chacha20Encrypt(const std::vector<uint8_t> & plain_text_message, const std::array<uint8_t, 32> & key, const std::array< uint8_t, 12> & nonce, const uint32_t counter);


void TestBlake2b();

int main()
{	
	
		
// /*
//                                                         Ladies and Gentl
//   016  65 6d 65 6e 20 6f 66 20 74 68 65 20 63 6c 61 73  emen of the clas
//   032  73 20 6f 66 20 27 39 39 3a 20 49 66 20 49 20 63  s of '99: If I c
//   048  6f 75 6c 64 20 6f 66 66 65 72 20 79 6f 75 20 6f  ould offer you o
//   064  6e 6c 79 20 6f 6e 65 20 74 69 70 20 66 6f 72 20  nly one tip for
//   080  74 68 65 20 66 75 74 75 72 65 2c 20 73 75 6e 73  the future, suns
//   096  63 72 65 65 6e 20 77 6f 75 6c 64 20 62 65 20 69  creen would be i
//   112  74 2e                                            t.
// */

std::string message = std::string("Ladies and Gentlemen of the class of \'99: If I would offer you only one tip for the future, sunscreen would be it.") ;

std::vector<uint8_t> plain_text_message;

static_assert(sizeof(char) == sizeof(uint8_t));

printf("\n Plain Text Message: \n");
for(int i = 0; i < message.size(); i++)
{
	 plain_text_message.push_back(static_cast<uint8_t>(message[i]));
	 printf("i : %d , %c,   %x \n", i, plain_text_message[i] ,  plain_text_message[i]);
}

std::array< uint8_t, 32> key = { 0x00 , 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 
	0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f };

std::array< uint8_t, 12> nonce = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4a, 0x00, 0x00, 0x00, 0x00};

const uint32_t counter = 1;

std::vector<uint8_t> cipher_text_message = Chacha20Encrypt(plain_text_message, key, nonce, counter);

printf("\n Cipher (Encrypted) Text Message: \n");

for(int i = 0; i < cipher_text_message.size(); i++)
{
	printf("\n i: %d, %x , %c ",i, cipher_text_message[i] , cipher_text_message[i]);
}

std::vector<uint8_t> decrypted_text_message = Chacha20Decrypt(cipher_text_message, key, nonce, counter);

printf("\n Decrypted Text Message: \n");

for(int i = 0; i < decrypted_text_message.size(); i++)
{
	printf("\n i: %d, %x , %c ",i, decrypted_text_message[i] , decrypted_text_message[i]);
}

//MAIN DOUBT: string's char to uint8_t , then uint8_t back to cipher text char..... ????


TestBlake2b();

uint64_t test_el = 0xabcd1122ff3412ac;
printf(" test_el:  %lx ", test_el);
printf(" \n sizeof(int) : %ld , sizeof(uint64_t): %ld ", sizeof(int), sizeof(uint64_t));

}
