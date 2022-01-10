// #include <iostream>  //already add to block_chain
// #include <chrono>    //already add to block_chain
#include "includes/rsa.hpp"

#define KEY_CLASS rsa_key_pair //we can use template as well

namespace ra
{
    class transaction
    {
    private:
        unsigned long long int from_address; //your public key
        unsigned long long int to_address;   //ohers public key
        float transfer_amount;
        std::chrono::steady_clock::time_point timestamp;
        std::size_t signature;

    public:
        transaction(const unsigned long long int from_address, const unsigned long long int to_address, const float &transfer_amount);
        std::string generate_hash_input();                                    //converts to string and concatenates them
        friend std::ostream &operator<<(std::ostream &, transaction const &); //to print transacttion
        float get_balance(const unsigned long long int);                      //makes sense for multiple transactions
        bool sign_transaction(KEY_CLASS key_pair);                            //hash of input happen during signing
        bool is_transaction_valid();                                          //uses verify of rsa
    };

    transaction::transaction(const unsigned long long int from_address, const unsigned long long int to_address, const float &transfer_amount)
    {
        this->from_address = from_address;
        this->to_address = to_address;
        this->transfer_amount = transfer_amount;
        timestamp = std::chrono::steady_clock::now();
        signature = 0;
    }

    std::string transaction::generate_hash_input()
    {
        std::string hash_input = std::to_string((std::chrono::duration_cast<std::chrono::microseconds>(this->timestamp.time_since_epoch()).count()));
        hash_input += std::to_string(this->from_address) + std::to_string(this->to_address) + std::to_string(this->transfer_amount);
        return hash_input;
    }

    std::ostream &operator<<(std::ostream &out, transaction const &temp)
    {
        out << "\nfrom_address: " << temp.from_address;
        out << "\nto_address: " << temp.to_address;
        out << "\ntransfer_amount: " << temp.transfer_amount;
        out << "\nsignature: " << temp.signature;
        return out;
    }

    float transaction::get_balance(unsigned long long int address)
    {
        if (address == this->to_address)
            return transfer_amount;

        if (address == this->from_address)
            return (-transfer_amount);

        return 0;
    }

    bool transaction::sign_transaction(KEY_CLASS key_pair)
    {
        if (from_address != key_pair.get_public_key())
        {
            std::cout << "address and key donot match";
            exit(0);
        }

        //already signed
        if (signature != 0)
            return false;

        signature = key_pair.sign(generate_hash_input());
        return true;
    }

    bool transaction::is_transaction_valid()
    {
        //reward
        if (from_address == 0)
            return true;

        //unsigned and not reward
        if (signature == 0)
            return false;

        return verify(generate_hash_input(), signature, from_address);
    }
}