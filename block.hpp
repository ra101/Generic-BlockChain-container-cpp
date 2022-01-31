#include <chrono>

#define MIN_DIFFICULTY 1
#define MED_DIFFICULTY 3
#define MAX_DIFFICULTY 3

namespace ra
{

    template <class Transaction, class HashFunctionClass>
    class block
    {
    private:
        int id;
        std::size_t previous_hash;
        std::size_t hash;

        // block and transaction both require timestamp
        std::chrono::steady_clock::time_point timestamp;

        // template input as here std::hash is used but u can make sha256 and the inetegrate them
        HashFunctionClass hash_function;

        // random data
        int nonce;

        // pending transactin will become tranctionlist here
        std::list<Transaction> transaction_list;

        // for proof of work
        long long int find_hash_length(int difficulty)
        {
            int factor = 1;
            for (int i = 0; i < difficulty; i++)
            {
                factor *= 10;
            }

            // hash functor produces a number of 10 or 9 digits
            return (100000000000000 / factor);
        }

        // converion to string and concatenation of data
        std::string generate_hash_input()
        {
            std::string hash_input = std::to_string((std::chrono::duration_cast<std::chrono::microseconds>(timestamp.time_since_epoch()).count()));
            for (typename std::list<Transaction>::iterator trans_elem = transaction_list.begin(); trans_elem != transaction_list.end(); trans_elem++)
            {
                hash_input += (*trans_elem).generate_hash_input();
            }
            hash_input += std::to_string(previous_hash);
            return hash_input;
        }

    public:
        block(std::size_t previous_hash, int difficulty, std::list<Transaction> transaction_list, int id)
        {
            this->id = id;
            this->previous_hash = previous_hash;
            this->transaction_list = transaction_list;
            difficulty = std::max(std::min(difficulty, MAX_DIFFICULTY), MIN_DIFFICULTY);
            nonce = 0;
            timestamp = std::chrono::steady_clock::now();
            hash = mine_block(generate_hash_input(), find_hash_length(difficulty));
        }
        std::size_t mine_block(std::string hash_input, long long int hash_length)
        {
            std::size_t hash_output = hash_function(hash_input + std::to_string(nonce));
            while (hash_output / hash_length != 0) // own time of proof of work
            {
                nonce += 1;
                hash_output = hash_function(hash_input + std::to_string(nonce));
            }
            return hash_output;
        }
        std::size_t get_hash()
        {
            return hash;
        }

        std::size_t get_previous_hash()
        {
            return previous_hash;
        }

        // recalculates hash and matches to current hash
        bool is_hash_valid()
        {
            std::string hash_input = generate_hash_input();
            hash_input += std::to_string(nonce);
            return (hash == hash_function(hash_input));
        }

        template <typename Message>
        bool is_block_valid(bool (*verfication_function)(Message, std::size_t, unsigned long long int))
        {

            for (typename std::list<Transaction>::iterator trans_elem = transaction_list.begin(); trans_elem != transaction_list.end(); trans_elem++)
                if (!((*trans_elem).is_transaction_valid(verfication_function)))
                    return false;
            return true;
        }

        // traverse thorugh all transaction
        float get_balance(const unsigned long long int address)
        {
            float balance = 0;
            for (typename std::list<Transaction>::iterator trans_elem = transaction_list.begin(); trans_elem != transaction_list.end(); trans_elem++)
                balance += (*trans_elem).get_balance(address);
            return balance;
        }

        // modifiable.
        friend std::ostream &operator<<(std::ostream &out, block<Transaction, HashFunctionClass> const &temp)
        {
            int i = 1;
            if (temp.id == 0)
                out << "...\nGenesis block\n";
            else
                out << "\n\n...\nBlock No. " << temp.id;

            out << "\n\nNo of Entries: " << temp.transaction_list.size() << "\nHash: " << temp.hash << "\nPrevious Hash: " << temp.previous_hash;
            for (typename std::list<Transaction>::const_iterator trans_elem = (temp.transaction_list).begin(); trans_elem != temp.transaction_list.end(); trans_elem++, i++)
                out << "\n\nEntry no: " << i << (*trans_elem);

            return out;
        }
    };

}
