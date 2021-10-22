#include<iostream>
#include<vector>
#include<array>
#include<string>


std::array<  uint8_t, 64> Chacha20Block( const std::array< uint8_t, 32> & key, const std::array< uint8_t, 12> & nonce, const uint32_t counter);



std::array<uint8_t, 32> Poly1305KeyGeneration(const std::array<uint8_t, 32> & key, const std::array< uint8_t, 12> & nonce)
{
	const uint32_t counter = 0;
	const std::array<uint8_t, 64> block =  Chacha20Block(key, nonce, counter);
	std::array<uint8_t, 32> mac_key;

	for(int i = 0; i < 32; i++)
	{
		mac_key[i] = block[i];
	}
	return mac_key;
}




std::array<uint8_t, 16> Get_r(const std::array<uint8_t, 32> & mac_key)
{
	std::array<uint8_t, 16> r;
	for(int i = 0; i < 16; i++)
	{
		r[i] = mac_key[i];
	}
}

void Clamp(std::array<uint8_t, 16> & r)
{
    r[3] &= 0x0f;
    r[7] &= 0x0f;
    r[11] &= 0x0f;
    r[15] &= 0x0f;

    r[4] &= 0xfc;
    r[8] &= 0xfc;
    r[12] &= 0xfc;
}

std::array<uint8_t, 16> Get_s(const std::array<uint8_t, 32> & mac_key)
{
	std::array<uint8_t, 16> s;
	for(int i = 16; i < 32; i++)
	{
		s[i] = mac_key[i];
	}
}




std::array<uint8_t, 16> Poly1305Mac(const std::vector<uint8_t> & message, const std::array<uint8_t, 32> & mac_key)
{	
	std::array<uint8_t, 16> r = Get_r(mac_key);
	Clamp(r);
	std::array<uint8_t, 16> s = Get_s(mac_key);	

	std::array<uint8_t, 16> tag;
	return tag;
}