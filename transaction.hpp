#include <iostream>
#include <chrono>

namespace ra
{
    class base_transaction
    {
    public:
        std::string generate_hash_input();                                         // converts to string and concatenates them
        friend std::ostream &operator<<(std::ostream &, base_transaction const &); // to print transacttion
        float get_balance(const unsigned long long int);
        template <typename EncryptionAlgo>              // makes sense for multiple transactions
        bool sign_transaction(EncryptionAlgo key_pair); // hash of input happen during signing

        template <typename Message>                                                        // makes sense for multiple transactions
        bool is_transaction_valid(bool (*)(Message, std::size_t, unsigned long long int)); // uses verify of rsa
    };
}
