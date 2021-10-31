#include<iostream>
#include<vector>
#include<array>
#include<string>


namespace{

template<int arr_size, int start_idx>
void Pack4(const std::array< uint8_t, arr_size> & arr,  uint32_t & chacha20_state_element)
{
	static_assert(start_idx >= 0);
	static_assert(start_idx + 3  <= arr_size - 1);

	const uint8_t octet1 = arr[start_idx];
	const uint8_t octet2 = arr[start_idx + 1];
	const uint8_t octet3 = arr[start_idx + 2];
	const uint8_t octet4 = arr[start_idx + 3];

	// state_element =  octet4 CONCAT octet3 CONCAT octet2 CONCAT octet1

	uint32_t oc1 = static_cast<uint32_t>(octet1); 
	//printf("\n octet1 : %x , %x ", octet1 , oc1);  
	
	uint32_t oc2 = static_cast<uint32_t>(octet2);
	//printf("\n octet2 : %x , %x ", octet2 , oc2);
	oc2 = oc2 << 8;
	//printf("\n octet2 : %x , %x ", octet2 , oc2);

	uint32_t oc3 = static_cast<uint32_t>(octet3);
	//printf("\n octet3 : %x , %x ", octet3 , oc3);
	oc3 = oc3 << 16;
	//printf("\n octet3 : %x , %x ", octet3 , oc3);

	uint32_t oc4 = static_cast<uint32_t>(octet4);
	//printf("\n octet4 : %x , %x ", octet4 , oc4);
	oc4 = oc4 << 24;
	//printf("\n octet4 : %x , %x ", octet4 , oc4);

	chacha20_state_element =  oc4 | oc3 | oc2 | oc1;

	//printf("\n finally: %x", chacha20_state_element);

	/* doubt: Is the shift circular?  */
}


template<int arr_size , int start_idx>
void Unpack_to_4(const uint32_t element,  std::array<uint8_t, arr_size> & arr)
{
	static_assert(start_idx >= 0);
	static_assert(start_idx + 3 <= arr_size - 1);

	//printf("\n element: %x ", element);

	//Look at :  element = octet4 CONCAT octet3 CONCAT octet2 CONCAT octet1
	uint32_t oc1 = element;
	oc1 = oc1 & 0x000000ff;
	// printf("\n oc1: %x", oc1);

	uint32_t oc2 = element;
	oc2 = oc2 & 0x0000ff00;
	// printf("\n oc2: %x ", oc2);
	oc2 = oc2 >> 8;
	// printf("\n oc2: %x ", oc2);

	uint32_t oc3 = element;
	oc3 = oc3 & 0x00ff0000;
	// printf("\n oc3: %x", oc3);
	oc3 = oc3 >> 16;
	// printf("\n oc3: %x", oc3);

	uint32_t oc4 = element;
	oc4 = oc4 & 0xff000000;
	// printf("\n oc4: %x",oc4);
	oc4 = oc4 >> 24;
	// printf("\n oc4: %x",oc4);
	
	uint8_t octet1 = static_cast<uint8_t>(oc1);
	uint8_t octet2 = static_cast<uint8_t>(oc2);
	uint8_t octet3 = static_cast<uint8_t>(oc3);
	uint8_t octet4 = static_cast<uint8_t>(oc4);

	// printf("\n octet1: %x", octet1);
	// printf("\n octet2: %x", octet2);
	// printf("\n octet3: %x", octet3);
	// printf("\n octet4: %x", octet4);
	
	arr[start_idx] = octet1;
	arr[start_idx + 1] = octet2;
	arr[start_idx + 2] = octet3;
	arr[start_idx + 3] = octet4;
}


void PrintChacha20State(const std::array< uint32_t, 16> & chacha20_state)
{
	for(int i = 0; i < 16; i++)
	{
		printf("\n i: %d, ele: %x ", i, chacha20_state[i]);
	}
}

void InitializeState( const std::array<uint8_t, 32> & key, const std::array<uint8_t, 12> & nonce, const uint32_t block_counter, std::array< uint32_t, 16> & chacha20_initial_state )
{
	/*
	   0: cccccccc    1: cccccccc   2: cccccccc   3: cccccccc
       4: kkkkkkkk    5: kkkkkkkk   6: kkkkkkkk   7: kkkkkkkk
       8: kkkkkkkk    9: kkkkkkkk   10: kkkkkkkk  11: kkkkkkkk
       12: bbbbbbbb   13: nnnnnnnn  14: nnnnnnnn  15: nnnnnnnn
	*/

	chacha20_initial_state[0] = 0x61707865; 
	chacha20_initial_state[1] = 0x3320646e;
	chacha20_initial_state[2] = 0x79622d32;
	chacha20_initial_state[3] = 0x6b206574;

	Pack4<32, 0>(key, chacha20_initial_state[4]); //starting from index:0 in key array, pack 4 elements to get 32 bits for the chacha state
	Pack4<32, 4>(key, chacha20_initial_state[5]);
	Pack4<32, 8>(key, chacha20_initial_state[6]);
	Pack4<32, 12>(key, chacha20_initial_state[7]);
	Pack4<32, 16>(key, chacha20_initial_state[8]);
	Pack4<32, 20>(key, chacha20_initial_state[9]);
	Pack4<32, 24>(key, chacha20_initial_state[10]);
	Pack4<32, 28>(key, chacha20_initial_state[11]);

	chacha20_initial_state[12] = block_counter;

	Pack4<12, 0>(nonce, chacha20_initial_state[13]);
	Pack4<12, 4>(nonce, chacha20_initial_state[14]);
	Pack4<12, 8>(nonce, chacha20_initial_state[15]);

}


void CircularLeftShift(uint32_t & element, const int shift)
{
	element = (element << shift) | (element >> (32 - shift));
}

template <int a, int b, int c, int d>
void QuarterRound(std::array<uint32_t, 16> & chacha20_state)
{
	static_assert( a >= 0 && a < 16);
	static_assert( b >= 0 && b < 16);
	static_assert(c >= 0 && c < 16);
	static_assert( d >= 0 && d < 16);

	// a += b; d ^= a; d <<<= 16;
	chacha20_state[a] += chacha20_state[b];
	chacha20_state[d] ^= chacha20_state[a];
	CircularLeftShift(chacha20_state[d], 16);
	
	// c += d; b ^= c; b <<<= 12;
	chacha20_state[c] += chacha20_state[d];
	chacha20_state[b] ^= chacha20_state[c];
	CircularLeftShift(chacha20_state[b], 12); 

	// a += b; d ^= a; d <<<= 8;
	chacha20_state[a] += chacha20_state[b];
	chacha20_state[d] ^= chacha20_state[a];
	CircularLeftShift(chacha20_state[d], 8);
	
	// c += d; b ^= c; b <<<= 7;
	chacha20_state[c] += chacha20_state[d];
	chacha20_state[b] ^= chacha20_state[c];
	CircularLeftShift(chacha20_state[b], 7);
	
}

void InnerBlock(std::array<uint32_t, 16> & chacha20_state)
{
	QuarterRound<0, 4, 8, 12>(chacha20_state);
	QuarterRound<1, 5, 9, 13>(chacha20_state);
	QuarterRound<2, 6, 10, 14>(chacha20_state);
	QuarterRound<3, 7, 11, 15>(chacha20_state);
	QuarterRound<0, 5, 10, 15>(chacha20_state);
	QuarterRound<1, 6, 11, 12>(chacha20_state);
	QuarterRound<2, 7, 8, 13>(chacha20_state);
	QuarterRound<3, 4, 9, 14>(chacha20_state);
}

void AddInitialStateToState(const std::array< uint32_t, 16> & chacha20_initial_state,  std::array<uint32_t, 16> & chacha20_state)
{
	//state = state + initial_state
	for(int i = 0; i < chacha20_state.size(); i++)
	{
		chacha20_state[i] = chacha20_initial_state[i] + chacha20_state[i];
	}
}


void Serialize(const std::array< uint32_t, 16> & chacha20_state, std::array< uint8_t, 64> & key_stream)
{
	Unpack_to_4<64, 0>(chacha20_state[0], key_stream);
	Unpack_to_4<64, 4>(chacha20_state[1], key_stream);
	Unpack_to_4<64, 8>(chacha20_state[2], key_stream);
	Unpack_to_4<64, 12>(chacha20_state[3], key_stream);
	Unpack_to_4<64, 16>(chacha20_state[4], key_stream);
	Unpack_to_4<64, 20>(chacha20_state[5], key_stream);
	Unpack_to_4<64, 24>(chacha20_state[6], key_stream);
	Unpack_to_4<64, 28>(chacha20_state[7], key_stream);
	Unpack_to_4<64, 32>(chacha20_state[8], key_stream);
	Unpack_to_4<64, 36>(chacha20_state[9], key_stream);
	Unpack_to_4<64, 40>(chacha20_state[10], key_stream);
	Unpack_to_4<64, 44>(chacha20_state[11], key_stream);
	Unpack_to_4<64, 48>(chacha20_state[12], key_stream);
	Unpack_to_4<64, 52>(chacha20_state[13], key_stream);
	Unpack_to_4<64, 56>(chacha20_state[14], key_stream);
	Unpack_to_4<64, 60>(chacha20_state[15], key_stream);

	//for(int i = 0; )

}

void PrintKeyStream(const std::array< uint8_t, 64> & key_stream)
{
	for(int i = 0; i < key_stream.size(); i++)
	{
		printf("\n key_stream[%d]: %x ", i, key_stream[i] );
	}
}


std::array<  uint8_t, 64> Chacha20Block( const std::array< uint8_t, 32> & key, const std::array< uint8_t, 12> & nonce, const uint32_t counter)
{
	std::array< uint32_t, 16> chacha20_initial_state;
	InitializeState(key, nonce, counter, chacha20_initial_state);

	//printf("\n Initial chacha state: \n");
	//PrintChacha20State(chacha20_initial_state);

	std::array< uint32_t, 16> chacha20_state = chacha20_initial_state;
	for(int i = 0; i < 10; i++)
	{
		InnerBlock(chacha20_state);
	}

	//printf("\n\n Chacha state after 20 rounds: ");
	//PrintChacha20State(chacha20_state);

	AddInitialStateToState(chacha20_initial_state, chacha20_state);

	//printf("\n\n Final Chacha state:");
	//PrintChacha20State(chacha20_state);

	std::array< uint8_t, 64> key_stream;
	Serialize( chacha20_state, key_stream);

	//PrintKeyStream(key_stream);
	return key_stream;
}





void XOR_operation_with_keystream_block(std::vector<uint8_t> & cipher_text, const int start_idx, const int end_idx, const std::array<uint8_t, 64> & key_stream)
{
	for(int i = start_idx; i < end_idx; i++)
	{
		cipher_text[i] ^= key_stream[i - start_idx];
	}
}

} //unnamed namespace

std::vector<uint8_t> Chacha20Encrypt(const std::vector<uint8_t> & plain_text_message, const std::array<uint8_t, 32> & key, const std::array< uint8_t, 12> & nonce, const uint32_t counter)
{	
	std::vector<uint8_t> cipher_text = plain_text_message;

	const int num_covers = plain_text_message.size()/64;

	for(int i = 0; i < num_covers; i++)
	{	
		std::array<uint8_t, 64> key_stream =  Chacha20Block(key, nonce, counter + i);
		XOR_operation_with_keystream_block(cipher_text, i * 64, (i + 1)*64,  key_stream);
	}

	std::array<uint8_t, 64> key_stream =  Chacha20Block(key, nonce, counter + num_covers);
	XOR_operation_with_keystream_block(cipher_text, num_covers * 64 , cipher_text.size(), key_stream);

	return cipher_text;
}


std::vector<uint8_t> Chacha20Decrypt(const std::vector<uint8_t> & cipher_text_message, const std::array<uint8_t, 32> & key, const std::array< uint8_t, 12> & nonce, const uint32_t counter)
{	
	std::vector<uint8_t> plain_text = cipher_text_message;

	const int num_covers = cipher_text_message.size()/64;

	for(int i = 0; i < num_covers; i++)
	{	
		std::array<uint8_t, 64> key_stream =  Chacha20Block(key, nonce, counter + i);
		XOR_operation_with_keystream_block(plain_text, i * 64, (i + 1)*64,  key_stream);
	}

	std::array<uint8_t, 64> key_stream =  Chacha20Block(key, nonce, counter + num_covers);
	XOR_operation_with_keystream_block(plain_text, num_covers * 64 , plain_text.size(), key_stream);

	return plain_text;
}




void Test()
{
	/*
	std::array<uint8_t, 4> arr = {0x12 , 0x34 , 0xff , 0xab };
	uint32_t ele;
	Pack4<4, 0>(arr, ele);

	uint32_t element = 0xab2345f1;
	std::array<uint8_t, 8> arr_new;
	Unpack_to_4<8 , 4>(element, arr_new);


	uint32_t a = 0x61707865 + 0x837778ab;
	printf("\n\n a: %x", a);

	a = 0xffffffff + 0xffffffff;
	printf("\n\n a: %u", a);  //2^32 addition means ????

	uint64_t b = (uint64_t )0xffffffff + (uint64_t )0xffffffff;
	printf("\n\n b: %u", b);
	*/


// 	std::array< uint8_t, 32> key = { 0x00 , 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 
// 		0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f };
// 	std::array< uint8_t, 12> nonce = { 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x4a, 0x00, 0x00, 0x00, 0x00};
// 	Chacha20Block(key , nonce , 1);


// 	  uint32_t a = 0x11111111;
//       uint32_t b = 0x01020304;
//       uint32_t c = 0x77777777;
//       uint32_t d = 0x01234567;
//       /*
//       c = c + d = 0x77777777 + 0x01234567 = 0x789abcde
//       b = b ^ c = 0x01020304 ^ 0x789abcde = 0x7998bfda
//       b = b <<< 7 = 0x7998bfda <<< 7 = 0xcc5fed3c
//       */

//       c = c + d;
//       printf("\nc = %x ", c);

//       b = b ^ c;
//       printf("\n b = %x ", b);

//       CircularLeftShift(b,7);
//       //b = b << 7;
//       printf("\n b = %x ", b);

}



