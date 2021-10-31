#include<iostream>
#include<array>
#include<vector>
#include<string>
#include<cassert>


namespace{

uint64_t CircularRightShift(const uint64_t bitset, const size_t shift)
{
	return ( (bitset >> shift) | ( bitset << (64 - shift) ) );
}

//Mixing function
template <int a, int b, int c, int d>
void Mix(const uint64_t  x, const uint64_t y, std::array<uint64_t, 16> & v)
{
	static_assert(a >= 0 && a <= 16);
	static_assert(b >= 0 && b <= 16);
	static_assert(c >= 0 && c <= 16);
	static_assert(d >= 0 && d <= 16);

	v[a] = v[a] + v[b] + x;
	v[d] = CircularRightShift(v[d] ^ v[a], 32);
	v[c] = v[c] + v[d];
	v[b] = CircularRightShift(v[b] ^ v[c], 24);
	v[a] = v[a] + v[b] + y;
	v[d] = CircularRightShift(v[d] ^ v[a], 16);
	v[c] = v[c] + v[d];
	v[b] = CircularRightShift(v[b] ^ v[c], 63);
}

template< int arr_size >
void Print_array(const std::array<uint64_t, arr_size> & arr)
{
	for(int i = 0; i < arr_size; i++)
	{
		printf("\n arr[%d]: %lx ", i, arr[i]);
	}
}

//Compression function, manipulates h (internal hash state)
void Compress( const std::array<uint64_t, 16> & input_data_block, const std::array<uint64_t, 2> & t, const bool is_last_block, 
	const std::array<uint64_t, 8> & IV, const std::array< std::array< size_t, 16> , 10> & sigma,  std::array<uint64_t, 8> & h)

{

	std::array<uint64_t, 16> v; //local work vector
	//Initialize local work vector
	for(int i = 0; i < 8; i++)
	{
		v[i] = h[i];
	}

	for(int i = 8; i < 16; i++)
	{
		v[i] = IV[i - 8];
	}

	v[12] ^= t[0];
	v[13] ^= t[1];

	if(is_last_block == true)
	{
		v[14] = ~v[14];
	}

	for(int round = 0; round < 12; round++)
	{	
		printf("\n round: %d", round);
		Print_array<16>(v);

		Mix<0,4,8,12>(input_data_block[sigma[round % 10][0]], input_data_block[sigma[round % 10][1]], v);
		Mix<1,5,9,13>(input_data_block[sigma[round % 10][2]], input_data_block[sigma[round % 10][3]], v);
		Mix<2,6,10,14>(input_data_block[sigma[round % 10][4]], input_data_block[sigma[round % 10][5]], v);
		Mix<3,7,11,15>(input_data_block[sigma[round % 10][6]], input_data_block[sigma[round % 10][7]], v);

		Mix<0,5,10,15>(input_data_block[sigma[round % 10][8]], input_data_block[sigma[round % 10][9]], v);
		Mix<1,6,11,12>(input_data_block[sigma[round % 10][10]], input_data_block[sigma[round % 10][11]], v);
		Mix<2,7,8,13>(input_data_block[sigma[round % 10][12]], input_data_block[sigma[round % 10][13]], v);
		Mix<3,4,9,14>(input_data_block[sigma[round % 10][14]], input_data_block[sigma[round % 10][15]], v);
	}

    printf("\n after all rounds");
	Print_array<16>(v);


	for(int i =0; i < 8; i++)
	{
		h[i] ^= (v[i] ^ v[i + 8]);
	}

	printf("\n Hash: \n");
	Print_array<8>(h);

	


}




template<int arr_size, int start_idx>
void Pack8(const std::array< uint8_t, arr_size> & arr,  uint64_t & element)
{
	static_assert(start_idx >= 0);
	static_assert(start_idx + 7  <= arr_size - 1);

	const uint8_t octet1 = arr[start_idx];
	const uint8_t octet2 = arr[start_idx + 1];
	const uint8_t octet3 = arr[start_idx + 2];
	const uint8_t octet4 = arr[start_idx + 3];
	const uint8_t octet5 = arr[start_idx + 4];
	const uint8_t octet6 = arr[start_idx + 5];
	const uint8_t octet7 = arr[start_idx + 6];
	const uint8_t octet8 = arr[start_idx + 7];

	// state_element =   octet8 CONCAT octet7 CONCAT octet6 CONCAT octet5 CONCAT octet4 CONCAT octet3 CONCAT octet2 CONCAT octet1

	uint64_t oc1 = static_cast<uint64_t>(octet1); 
	//printf("\n octet1 : %x , %x ", octet1 , oc1);  
	
	uint64_t oc2 = static_cast<uint64_t>(octet2);
	oc2 = oc2 << 8;
	
	uint64_t oc3 = static_cast<uint64_t>(octet3) << 16;
	
	uint64_t oc4 = static_cast<uint64_t>(octet4) << 24;

	uint64_t oc5 = static_cast<uint64_t>(octet5) << 32;
	
	uint64_t oc6 = static_cast<uint64_t>(octet6) << 40;
	
	uint64_t oc7 = static_cast<uint64_t>(octet7) << 48;

	uint64_t oc8 = static_cast<uint64_t>(octet8) << 56;

	element =  oc8 | oc7 | oc6 | oc5 | oc4 | oc3 | oc2 | oc1;

	printf("\n\n\n octet1: %x , octet2: %x, octet3: %x , octet4: %x, octet5: %x, octet6: %x, octet7: %x , octet8: %x \n", octet1, octet2, octet3, octet4, octet5, octet6, octet7, octet8);
	printf("oc1: %lx , oc2: %lx, oc3: %lx , oc4: %lx, oc5: %lx, oc6: %lx, oc7: %lx , oc8: %lx \n", oc1, oc2, oc3, oc4, oc5, oc6, oc7, oc8);
	printf(" ele: %lx ", element);

	/* doubt: Is the shift circular?  */
}


template<int arr_size , int start_idx>
void Unpack_to_8(const uint64_t element, std::array<uint8_t, arr_size> & arr)
{
	static_assert(start_idx >= 0);
	static_assert(start_idx + 7 <= arr_size - 1);

	printf("\n element to be unpacked to 8: %lx ", element);

	//Look at :  element =  octet8 CONCAT octet7 CONCAT octet6 CONCAT octet5 CONCAT octet4 CONCAT octet3 CONCAT octet2 CONCAT octet1
	uint64_t oc1 = element;
	oc1 = oc1 & 0xff;
	 printf("\n oc1: %lx", oc1);

	uint64_t oc2 = element;
	oc2 = oc2 & 0xff00;
	// printf("\n oc2: %x ", oc2);
	oc2 = oc2 >> 8;
	 printf("\n oc2: %lx ", oc2);

	uint64_t oc3 = element;
	oc3 = oc3 & 0xff0000;
	// printf("\n oc3: %x", oc3);
	oc3 = oc3 >> 16;
	 printf("\n oc3: %lx", oc3);

	uint64_t oc4 = element;
	oc4 = oc4 & 0xff000000;
	// printf("\n oc4: %x",oc4);
	oc4 = oc4 >> 24;
	 printf("\n oc4: %lx",oc4);


	uint64_t oc5 = element;
	oc5 = oc5 & 0xff00000000;
	oc5 = oc5 >> 32;
	 printf("\n oc5: %lx",oc5);

	uint64_t oc6 = element;
	oc6 = oc6 & 0xff0000000000;
	oc6 = oc6 >> 40;
	 printf("\n oc6: %lx",oc6);


	uint64_t oc7 = element;
	oc7 = oc7 & 0xff000000000000;
	oc7 = oc7 >> 48;
	 printf("\n oc7: %lx",oc7);

	uint64_t oc8 = element;
	oc8 = oc8 & 0xff00000000000000;
	oc8 = oc8 >> 56;
	 printf("\n oc8: %lx",oc8);

	uint8_t octet1 = static_cast<uint8_t>(oc1);
	uint8_t octet2 = static_cast<uint8_t>(oc2);
	uint8_t octet3 = static_cast<uint8_t>(oc3);
	uint8_t octet4 = static_cast<uint8_t>(oc4);
	uint8_t octet5 = static_cast<uint8_t>(oc5);
	uint8_t octet6 = static_cast<uint8_t>(oc6);
	uint8_t octet7 = static_cast<uint8_t>(oc7);
	uint8_t octet8 = static_cast<uint8_t>(oc8);


	// printf("\n octet1: %x", octet1);
	// printf("\n octet2: %x", octet2);
	// printf("\n octet3: %x", octet3);
	// printf("\n octet4: %x", octet4);
	
	arr[start_idx] = octet1;
	arr[start_idx + 1] = octet2;
	arr[start_idx + 2] = octet3;
	arr[start_idx + 3] = octet4;
	arr[start_idx + 4] = octet5;
	arr[start_idx + 5] = octet6;
	arr[start_idx + 6] = octet7;
	arr[start_idx + 7] = octet8;
}


std::vector<uint8_t> GetHashBytes( std::array<uint64_t, 8> h, const size_t hash_num_bytes)
{
	std::array<uint8_t, 64> hash_tag_bytes_full_array; //Get this full array
	Unpack_to_8<64, 0>(h[0], hash_tag_bytes_full_array);
	Unpack_to_8<64, 8>(h[1], hash_tag_bytes_full_array);
	Unpack_to_8<64, 16>(h[2], hash_tag_bytes_full_array);
	Unpack_to_8<64, 24>(h[3], hash_tag_bytes_full_array);
	Unpack_to_8<64, 32>(h[4], hash_tag_bytes_full_array);
	Unpack_to_8<64, 40>(h[5], hash_tag_bytes_full_array);
	Unpack_to_8<64, 48>(h[6], hash_tag_bytes_full_array);
	Unpack_to_8<64, 56>(h[7], hash_tag_bytes_full_array);

	std::vector<uint8_t> hash_tag_bytes;

	for(int i = 0; i < hash_num_bytes; i++)
	{
		hash_tag_bytes.push_back(hash_tag_bytes_full_array[i]);
	}

	return hash_tag_bytes;
}



std::array<uint64_t, 16> TreatBuffer(std::array<uint8_t, 128> buffer)
{
	std::array<uint64_t, 16> input_data_block;

	Pack8<128, 0>( buffer,  input_data_block[0]);
	Pack8<128, 8>( buffer,  input_data_block[1]);
	Pack8<128, 16>( buffer,  input_data_block[2]);
	Pack8<128, 24>( buffer,  input_data_block[3]);
	Pack8<128, 32>( buffer,  input_data_block[4]);
	Pack8<128, 40>( buffer,  input_data_block[5]);
	Pack8<128, 48>( buffer,  input_data_block[6]);
	Pack8<128, 56>( buffer,  input_data_block[7]);
	Pack8<128, 64>( buffer,  input_data_block[8]);
	Pack8<128, 72>( buffer,  input_data_block[9]);
	Pack8<128, 80>( buffer,  input_data_block[10]);
	Pack8<128, 88>( buffer,  input_data_block[11]);
	Pack8<128, 96>( buffer,  input_data_block[12]);
	Pack8<128, 104>( buffer,  input_data_block[13]);
	Pack8<128, 112>( buffer,  input_data_block[14]);
	Pack8<128, 120>( buffer,  input_data_block[15]);

	return input_data_block;
}



void CreateDataBlock(const std::vector<uint8_t> & hash_key , const std::vector<uint8_t> & message, size_t & increment_for_t, bool & is_last_block, std::array<uint64_t, 16> & input_data_block)
{	
	static size_t msg_ptr = 0;
	
	static size_t iter = 0;

	std::array<uint8_t, 128> buffer;

	is_last_block = false;
	increment_for_t = 128;

	if(iter == 0 &&  hash_key.size() == 0 && message.size() == 0 )
	{
		for(size_t i = 0; i < 128; i++)
		{
			buffer[i] = 0x00;
		}

		increment_for_t = 0;
		is_last_block = true;
	}
	else if(iter == 0 && hash_key.size() > 0 )
	{	
		printf("\n Creating data bloack from key\n");
		size_t i = 0;
		for(; i < hash_key.size(); i++)
		{
			buffer[i] = hash_key[i];
		}

		for(; i < 128; i++)
		{
			buffer[i] = 0x00;
		}

		iter++;

		if(message.size() == 0)
		{
			is_last_block = true;
			msg_ptr = 0;
			iter = 0;

		}

	}
	else if(iter > 0 || hash_key.size() == 0)
	{
		iter++;

		size_t i;
		for(i = msg_ptr; i < message.size() && i < msg_ptr + 128; i++)
		{
			buffer[i] = message[i];
		}

		if(i < msg_ptr + 128)
		{	
			increment_for_t = i - msg_ptr;
			while(i < msg_ptr + 128)
			{
				buffer[i] = 0x00;
				i++;
			}

			is_last_block = true;
			msg_ptr = 0;
			iter = 0;

		}
		else
		{
			is_last_block = false;
			msg_ptr = msg_ptr + 128;
		}
			
	}


	//buffer to data block
	input_data_block = TreatBuffer(buffer);

	

}

} //unnamed namespace

std::vector<uint8_t> Blake2b(const std::vector<uint8_t> & hash_key , const std::vector<uint8_t> & message, const size_t hash_num_bytes)
{	
	const size_t keylen = hash_key.size();

	printf("\n keylen: %d ", keylen);

	assert(keylen >= 0 &&  keylen <= 64);
	//assert(msglen >= 0 && msglen <= 2**128 -> v.big, exceeds max vector (of chars) size for this system)	
	assert(hash_num_bytes >= 1 && hash_num_bytes <= 64);


	const std::array<uint64_t, 8> IV = { 
		                                 0x6a09e667f3bcc908, 
		                                 0xbb67ae8584caa73b,
                                         0x3c6ef372fe94f82b, 
                                         0xa54ff53a5f1d36f1,
                                         0x510e527fade682d1,
                                         0x9b05688c2b3e6c1f,
                                         0x1f83d9abfb41bd6b,
                                         0x5be0cd19137e2179 };

	//DOUBT: const in inner array???
	// const std::array< std::array< size_t, 16> , 10> SIGMA = { { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12, 13, 14, 15 }, 
	// 													      { 14, 10, 4, 8, 9, 15, 13, 6, 1, 12, 0, 2, 11, 7, 5, 1 },  
	// 													      { 11 , 8, 12, 0, 5, 2 , 15, 13, 10 , 14, 3 , 6, 7, 1, 9 , 4 }, 
	// 													      { 7, 9 , 3, 1, 13 , 12, 11, 14, 2, 6, 5, 10 , 4 , 0 , 15, 8 }, 
	// 													      { 9, 0 , 5, 7, 2, 4, 10, 15 , 14 , 2,  11, 12 , 6 ,8 , 3, 13 }, 
	// 													      { 2, 12, 6, 10, 0, 11, 8, 3, 4, 13, 7, 5, 15, 14, 1, 9 },
	// 													      { 12 , 5, 1, 15 , 14 , 13 , 4, 10 , 0, 7, 6 ,3 ,9, 2, 8, 11 }, 
	// 													      { 13 , 11 , 7, 14 , 12, 1, 3, 9, 5, 0, 15, 4, 8, 6, 2, 10 }, 
	// 													      {6, 15, 14, 9, 11, 3, 0, 8, 12, 2, 13, 7, 1, 4, 10, 5},
	// 													      { 10, 2, 8, 4, 7, 6, 1, 5, 15, 11, 9,14 ,3 , 12 , 13, 0 } };

	std::array< std::array< size_t, 16> , 10> SIGMA;
	SIGMA[0] =  { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10,11,12, 13, 14, 15 };
	SIGMA[1] =  { 14, 10, 4, 8, 9, 15, 13, 6, 1, 12, 0, 2, 11, 7, 5, 3 };
	SIGMA[2] =  { 11 , 8, 12, 0, 5, 2 , 15, 13, 10 , 14, 3 , 6, 7, 1, 9 , 4 };
	SIGMA[3] =  { 7, 9 , 3, 1, 13 , 12, 11, 14, 2, 6, 5, 10 , 4 , 0 , 15, 8 };
	SIGMA[4] =  { 9, 0 , 5, 7, 2, 4, 10, 15 , 14 , 1,  11, 12 , 6 ,8 , 3, 13 };
	SIGMA[5] =  { 2, 12, 6, 10, 0, 11, 8, 3, 4, 13, 7, 5, 15, 14, 1, 9 };
	SIGMA[6] =  { 12 , 5, 1, 15 , 14 , 13 , 4, 10 , 0, 7, 6 ,3 ,9, 2, 8, 11 };
	SIGMA[7] =  { 13 , 11 , 7, 14 , 12, 1, 3, 9, 5, 0, 15, 4, 8, 6, 2, 10 };
	SIGMA[8] =  {6, 15, 14, 9, 11, 3, 0, 8, 12, 2, 13, 7, 1, 4, 10, 5};
	SIGMA[9] =  { 10, 2, 8, 4, 7, 6, 1, 5, 15, 11, 9,14 ,3 , 12 , 13, 0 };


	//Initialize internal hash state
	std::array<uint64_t, 8> h = IV;

	h[0] = h[0] ^ (   static_cast<uint64_t>(0x01010000) | (  static_cast<uint64_t>(keylen) << 8) | static_cast<uint64_t>(hash_num_bytes));

	std::array<uint64_t, 2> t = {0, 0};//input byte offset
	// t[0] has low word, t[1] has high word.     t is seen as numerical 128 bit number:  t[1] CONCAT t[0]
	
	bool is_last_block = false;
	
	while(is_last_block == false)
	{
		size_t increment_for_t;
		std::array<uint64_t, 16> input_data_block ;

		CreateDataBlock(hash_key,message, increment_for_t, is_last_block, input_data_block);
		
		printf("\n Is last block: %d ", is_last_block);

		for(int i = 0; i < 16; i++)
		{
			printf("\n input_data_block[%d]: %lx ", i, input_data_block[i]);
		}

		printf("\n increment_for_t: %d ", increment_for_t);
		
		t[0] += increment_for_t;
		if( t[0] < increment_for_t)  //carry overflow?
		{
			t[1] += 1; //high word
		}
		
		Compress(input_data_block, t, is_last_block, IV, SIGMA, h);
	}


	std::vector<uint8_t> hash_tag_bytes = GetHashBytes(h, hash_num_bytes);

	return hash_tag_bytes;

}

