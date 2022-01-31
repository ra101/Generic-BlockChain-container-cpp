#include "prime.hpp"
#include <random> // for template input of prime engine
//#include <chrono>  //for time seed //already added to block_chain.h

#define BIG_NO 100000000000
#define PAD_SIZE 10

namespace ra
{

    class rsa_key_pair
    {
    private:
        unsigned long long int private_key;
        unsigned long long int public_key;
        int get_e(int);
        int get_k(int, int);
        bool are_keys_valid();
        void create_key(unsigned long);

    public:
        rsa_key_pair();
        rsa_key_pair(std::string);
        std::size_t decrypt(unsigned);
        std::size_t decrypt_with_padding(std::string);
        unsigned long long int get_public_key();
        unsigned long long int get_private_key();
        template <typename Message>
        std::size_t sign(Message message);
    };
    // below function are nor the part of class neither friend function
    std::string padding(std::string);
    std::size_t encrypt(std::size_t, unsigned long long int);
    std::string encrypt_with_padding(std::size_t, unsigned long long int);
    template <typename Message>
    bool verify(Message, std::size_t, unsigned long long int);

    rsa_key_pair::rsa_key_pair()
    {
        std::chrono::steady_clock::time_point timestamp = std::chrono::steady_clock::now();
        unsigned long time_seed = std::chrono::duration_cast<std::chrono::nanoseconds>(timestamp.time_since_epoch()).count();
        create_key(time_seed);
    }

    rsa_key_pair::rsa_key_pair(std::string user_id)
    {
        std::hash<std::string> hash_function;
        unsigned long seed = hash_function(user_id);
        create_key(seed);
    }

    void rsa_key_pair::create_key(unsigned long seed)
    {
        random_prime_engine<std::default_random_engine> eng(seed);
        do
        {
            // two random prime int
            int p1 = eng();
            int p2 = eng();

            int n = p1 * p2;
            int phin = n - p1 - p2 + 1; //(p1-1)*(p2-1)
            int e = get_e(phin);
            int k = get_k(phin, e);
            int d = (k * phin + 1) / e;

            // multipplication to BIG_NO is way to concatenate n and e or d
            public_key = n * BIG_NO + e;
            private_key = n * BIG_NO + d;
        } while (!are_keys_valid());
    }

    int rsa_key_pair::get_e(int phin) // small odd no. which doesn't share factor with phin
    {
        for (int e = 3; e < phin; e += 2)
            if (phin % e != 0)
                return e;
    }

    int rsa_key_pair::get_k(int phin, int e) // such that k*phin +1 / e (=d) must be integer
    {
        for (int k = 1; k < phin; k += 1)
            if ((k * phin + 1) % e == 0)
                return k;
    }

    bool rsa_key_pair::are_keys_valid()
    {
        // 'n' of both no. are same
        return ((public_key / BIG_NO) == (private_key / BIG_NO));
    }

    unsigned long long int rsa_key_pair::get_public_key()
    {
        return public_key;
    }

    unsigned long long int rsa_key_pair::get_private_key()
    {
        return private_key;
    }

    std::size_t encrypt(std::size_t digest, unsigned long long int others_public_key)
    {
        int n = others_public_key / BIG_NO; // retrive n
        int e = others_public_key % BIG_NO; // retrive e
        long long int res = 1;              // Initialize result
        long long int x = digest;
        x = x % n; // Update x if it is more than or
                   // equal to n
        while (e > 0)
        {
            // If e is odd, multiply x with result
            if (e & 1)
                res = (res * x) % n;
            // e must be even now
            e = e >> 1; // e = e/2
            x = (x * x) % n;
        }
        return res;
    }

    std::size_t rsa_key_pair::decrypt(unsigned encrypted_data)
    {
        int n = private_key / BIG_NO; // retrive n
        int d = private_key % BIG_NO; // retrive d
        unsigned long long int res = 1;
        unsigned long long int x = encrypted_data;
        x = x % n;
        while (d > 0) // exact simmilar encrypt
        {
            if (d & 1)
                res = (res * x) % n;
            d = d >> 1;
            x = (x * x) % n;
        }
        return res;
    }

    std::string padding(std::string output_no)
    {
        while (output_no.length() < PAD_SIZE)
        {
            output_no = '0' + output_no; // add 0 infront of no.
        }
        return output_no;
    }

    std::string encrypt_with_padding(std::size_t digest, unsigned long long int others_public_key)
    {
        std::string output;
        while (digest != 0)
        {
            // encrypt one digit at a time and pad it
            output = padding(std::to_string(encrypt((digest % 10), others_public_key))) + output;
            digest /= 10;
        }
        return output;
    }

    std::size_t rsa_key_pair::decrypt_with_padding(std::string encrypted_data)
    {
        int digit = 0;
        std::size_t res = 0;
        int i, n = encrypted_data.length();
        for (i = 0; i < n; digit = 0)
        {
            while (i < n - 1 && encrypted_data.at(i) == '0') // let all zeros pass and n-1 'cuz of i+=1
            {
                i += 1;
                if (i % PAD_SIZE == 0)
                {
                    // if we pass a PAD_SIZEgth of '0's (encryption of zero is zero)
                    res = res * 10;
                }
            }
            while (i % PAD_SIZE != 0) // change loop after each PAD_SIZEgth
            {
                digit = digit * 10 + (encrypted_data.at(i) - '0'); // create digit
                i += 1;
            }
            res = res * 10 + decrypt(digit);
        }
        return res;
    }

    template <typename Message>
    std::size_t rsa_key_pair::sign(Message message)
    {
        std::hash<Message> hash_function;
        unsigned short int hash_output = hash_function(message); // short int 'cuz of sizeLimit(x*x) of decryption
        return (decrypt(hash_output));                           // signing is nothing but decrypt of hashed msg
    }

    template <typename Message>
    bool verify(Message message, std::size_t digest, unsigned long long int others_public_key)
    {
        std::hash<Message> hash_function;
        short int hash_output = hash_function(message);
        short int res = encrypt(digest, others_public_key);
        return (res == hash_output);
    }
}
