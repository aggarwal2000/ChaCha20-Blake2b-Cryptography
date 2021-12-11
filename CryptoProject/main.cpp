#include<iostream>
#include<vector>
#include<array>
#include<string>

#include"IO_utility.h"
#include"ChaCha20Cipher.h"
#include"Blake2b.h"
#include"Authentication_Encryption.h"

int main(int argc, char *argv[])
{	
	if(argc != 3)
	{	
		DemonstrateHMAC();
		exit(0);
	}
	
	bool to_authenticate = false;
	bool does_adversary_forge = false;

	if( atoi(argv[1]) != 0)
	{	
		to_authenticate = true;
	}

	if(atoi(argv[2]) != 0)
	{
		does_adversary_forge = true;
	}

	if(to_authenticate == true)
	{
		AuthenticatedEncryption(does_adversary_forge);
	}
	else
	{
		SoleEncryption(does_adversary_forge);
	}

}