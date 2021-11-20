#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<array>
#include<string>
#include<cassert>
#include <cstdio>
#include<filesystem>

namespace{



uint8_t HexCharToUint8(char c)
{
    uint8_t ans = 0;

    switch(c)
    {
        case '0':
        {   
            ans = 0;
            break;
        }
        case '1':
        {
            ans = 1;
            break;
        }
        case '2':
        {
            ans = 2;
            break;
        }
        case '3':
        {
            ans = 3;
            break;
        }
        case '4':
        {   
            ans = 4;
            break;
        }
        case '5':
        {
            ans = 5;
            break;
        }
        case '6':
        {
            ans = 6;
            break;
        }
        case '7':
        {
            ans = 7;
            break;
        }
         case '8':
        {   
            ans = 8;
            break;
        }
        case '9':
        {
            ans = 9;
            break;
        }
        case 'A':
        case 'a':
        {
            ans = 10;
            break;
        }
        case 'B':
        case 'b':
        {
            ans = 11;
            break;
        }
        case 'C':
        case 'c':
        {   
            ans = 12;
            break;
        }
        case 'D':
        case 'd':
        {
            ans = 13;
            break;
        }
        case 'E':
        case 'e':
        {
            ans = 14;
            break;
        }
        case 'F':
        case 'f':
        {
            ans = 15;
            break;
        }
        default:
        {
            assert(false);
        }
        
    }

    return ans;
}


char Uint8ToChar(const uint8_t num)
{
    char ans = '0';

    switch(num)
    {
        case 0:
        {   
            ans = '0';
            break;
        }
        case 1:
        {   
            ans = '1';
            break;
        }
         case 2:
        {   
            ans = '2';
            break;
        }
        case 3:
        {   
            ans = '3';
            break;
        }
        case 4:
        {   
            ans = '4';
            break;
        }
         case 5:
        {   
            ans = '5';
            break;
        }
        case 6:
        {   
            ans = '6';
            break;
        }
         case 7:
        {   
            ans = '7';
            break;
        }
        case 8:
        {   
            ans = '8';
            break;
        }
         case 9:
        {   
            ans = '9';
            break;
        }
        case 10:
        {   
            ans = 'a';
            break;
        }
        case 11:
        {   
            ans = 'b';
            break;
        }
         case 12:
        {   
            ans = 'c';
            break;
        }
        case 13:
        {   
            ans = 'd';
            break;
        }
        case 14:
        {   
            ans = 'e';
            break;
        }
        case 15:
        {   
            ans = 'f';
            break;
        }
        default:
        {
            assert(false);
        }
    }

    return ans;
}

std::string Uint8ToHexString(const uint8_t num)
{   
    std::string s_hex{};

    uint8_t number = num;

    uint8_t digit = number%16;
    number /=16;
    char lower = Uint8ToChar(digit);
    
    digit = number%16;
    number /=16;
    char higher = Uint8ToChar(digit);

    s_hex = std::string(1, higher) + std::string(1, lower);

    return s_hex;
}


void ReadPlainText(std::vector<uint8_t>& plain_text_message)
{
    std::string line;
    std::ifstream message_file("InputFiles/input_plain_txt_message.txt");
    if( message_file.is_open())
    {   
        bool data_read = false;

        while( std::getline(message_file, line) )
        {   
            data_read = true;
            for(int i = 0; i < line.size(); i++)
            {
                plain_text_message.push_back(static_cast<uint8_t>(line[i]));
            }

            plain_text_message.push_back(static_cast<uint8_t>('\n'));
        }

        if(data_read)
        {
            plain_text_message.pop_back();
        }
    }
    else
    {
        std::cout << "Unable to open file: input_plain_txt_message.txt" << std::endl;
        exit(1);
    }
}

void ReadChaChaKey(std::array< uint8_t, 32> & chacha20_key)
{
    std::string line;
    std::ifstream chacha_key("InputFiles/chacha20_key.txt");
    if( chacha_key.is_open())
    {   
        for(int i = 0; i < 32; i++)
        {
            std::getline(chacha_key, line); //This line is a hex line
            uint8_t byte;
            byte = static_cast<uint8_t>(16) * HexCharToUint8(line[0]) + HexCharToUint8(line[1]);
            chacha20_key[i] = byte;

        }
        
    }
    else
    {
        std::cout << "Unable to open file: chacha20_key.txt" << std::endl;
        exit(1);
    }
}

void ReadChaChaNonce(std::array< uint8_t, 12> & chacha20_nonce)
{   
    std::string line;
    std::ifstream chacha_nonce("InputFiles/chacha20_nonce.txt");
    if( chacha_nonce.is_open())
    {   
        for(int i = 0; i < 12; i++)
        {
            std::getline(chacha_nonce, line); //This line is a hex line
            uint8_t byte;
            byte = static_cast<uint8_t>(16) * HexCharToUint8(line[0]) + HexCharToUint8(line[1]);
            chacha20_nonce[i] = byte;
        }
    }
    else
    {
        std::cout << "Unable to open file: chacha20_nonce.txt" << std::endl;
        exit(1);
    }
}



void ReadChaChaCounter(uint32_t & chacha20_counter)
{
    std::string line;
    std::ifstream chacha_counter("InputFiles/chacha20_counter.txt");
    if( chacha_counter.is_open())
    {   
        std::getline(chacha_counter, line);
        chacha20_counter = stoi(line);
    }
    else
    {
        std::cout << "Unable to open file:chacha20_counter.txt" << std::endl;
        exit(1);
    }
}

void ReadHmacKey( std::vector<uint8_t>& hmac_key)
{
    std::string line;
    std::ifstream blake_key("InputFiles/blake2b-512_key.txt");
    if( blake_key.is_open())
    {   
        bool data_read = false;

        while( std::getline(blake_key, line) )
        {   
            data_read = true;
            for(int i = 0; i < line.size(); i++)
            {
                hmac_key.push_back(static_cast<uint8_t>(line[i]));
            }

            hmac_key.push_back(static_cast<uint8_t>('\n'));

        }

        if(data_read)
        {
            hmac_key.pop_back();
        }
       
    }
    else
    {
        std::cout << "Unable to open file:blake2b-512_key.txt" << std::endl;
    }


}

void ReadHashNumBytes(size_t & hash_num_bytes)
{
    std::string line;
    std::ifstream blake_digest_size("InputFiles/blake2b-512_digest_size.txt");
    if( blake_digest_size.is_open())
    {   
        std::getline(blake_digest_size, line);
        hash_num_bytes = stoi(line);
    }
    else
    {
        std::cout << "Unable to open file:blake2b-512_digest_size.txt" << std::endl;
        exit(1);
    }

}


} //unnamed namespace

void ReadInputFiles(std::vector<uint8_t>& plain_text_message, std::array< uint8_t, 32> & chacha20_key, std::array< uint8_t, 12> & chacha20_nonce, 
uint32_t & chacha20_counter, std::vector<uint8_t>& hmac_key, size_t & hash_num_bytes)
{
    static_assert(sizeof(char) == sizeof(uint8_t));
    ReadPlainText(plain_text_message);
    ReadChaChaKey(chacha20_key);
    ReadChaChaNonce(chacha20_nonce);
    ReadChaChaCounter(chacha20_counter);
    ReadHmacKey(hmac_key);
    ReadHashNumBytes(hash_num_bytes);
}


void ReadInputFiles(std::vector<uint8_t>& plain_text_message, std::array< uint8_t, 32> & chacha20_key, std::array< uint8_t, 12> & chacha20_nonce, 
uint32_t & chacha20_counter)
{
    static_assert(sizeof(char) == sizeof(uint8_t));
    ReadPlainText(plain_text_message);
    ReadChaChaKey(chacha20_key);
    ReadChaChaNonce(chacha20_nonce);
    ReadChaChaCounter(chacha20_counter);
}



void ReadInputFiles(std::vector<uint8_t>& text_message,std::vector<uint8_t>&  blake_key, size_t & digest_num_bytes)
{   
    static_assert(sizeof(char) == sizeof(uint8_t));
    ReadPlainText(text_message);
    ReadHmacKey(blake_key);
    ReadHashNumBytes(digest_num_bytes);
}



void write(const std::vector<uint8_t>& bytes, const bool is_str, const std::string & fname)
{
    std::ofstream myfile(fname);

    if(myfile.is_open())
    {
              for(size_t i = 0; i < bytes.size(); i++)
            {   
                if(is_str)
                {    
                    myfile << static_cast<char>(bytes[i]);
                }
                else
                {    
                    myfile << Uint8ToHexString(bytes[i]) << std::endl;
                }
            
            }
    }
    else
    {
         std::cout << "Unable to open file:" <<  fname  << std::endl;   
    }
  

}

void write(const std::string & to_write , const std::string & fname)
{
    std::ofstream myfile(fname);
    if(myfile.is_open())
    {   
        myfile << to_write;
    }
    else
    {
         std::cout << "Unable to open file:" <<  fname  << std::endl;   
    }

}



void DeleteFilesInDir(const std::string & dir)
{   
    
    if(std::filesystem::remove_all( dir ) == false)
    {   
        printf("\n Fails to delete dir files");
    }

    if(  std::filesystem::create_directory(dir) == false)
    {   
        printf("\n Fails to create dir");   
    }
    
}