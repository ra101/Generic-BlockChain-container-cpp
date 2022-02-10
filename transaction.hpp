#include <iostream>

#include "exceptions.hpp"

namespace ra
{
    class base_transaction
    {
    public:
        base_transaction(){};

        // from_address, to_address, transfer_amount
        base_transaction(const unsigned long long int, const unsigned long long int, const float &) { throw not_implemented_exception(); }

        // converts to string and concatenates them
        std::string generate_hash_input() const { throw not_implemented_exception(); }

        // to print transacttion
        operator std::string() const { throw not_implemented_exception(); }

        // to print transacttion
        friend std::ostream &operator<<(std::ostream &, base_transaction const &)
        {
            throw not_implemented_exception();
        }

        // used for comparision within a set
        bool operator<(const base_transaction &other) const { throw not_implemented_exception(); }

        // makes sense for multiple transactions
        float get_balance(const unsigned long long int) const { throw not_implemented_exception(); }

        // hash of input happen during signing
        template <typename EncryptionAlgo>
        bool sign_transaction(EncryptionAlgo key_pair)
        {
            throw not_implemented_exception();
        }

        // uses verify of rsa
        bool is_transaction_valid(bool (*)(std::string, std::size_t, unsigned long long int)) const
        {
            throw not_implemented_exception();
        }
    };
}
