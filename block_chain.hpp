#include <iostream>
#include <list>

#include "block.hpp"
// Each class is independed of other there exits no herachy yet data can't be manipulated due to checks

namespace ra
{

    // HashFunctionClass has no funtionality in this header, but it has to be passed into block constructor
    template <class Transaction, class HashFunctionClass>
    class block_chain
    {
    private:
        std::list<ra::block<Transaction, HashFunctionClass>> chain;
        int difficulty;
        float minning_reward;
        std::list<Transaction> pending_transaction;

        // It exists for a non reward based block_chain
        bool reward_flag;

    public:
        block_chain(int difficulty, float minning_reward)
        {
            this->difficulty = difficulty;
            this->minning_reward = minning_reward;
            chain.push_back(*(new block<Transaction, HashFunctionClass>(0, difficulty, pending_transaction, chain.size())));
            reward_flag = true;
        }

        block_chain(int difficulty)
        {
            this->difficulty = difficulty;
            chain.push_back(*(new block<Transaction, HashFunctionClass>(0, difficulty, pending_transaction, chain.size())));
            reward_flag = false;
        }
        bool is_chain_valid()
        {
            for (typename std::list<block<Transaction, HashFunctionClass>>::iterator previous_block, current_block = chain.begin(); current_block != chain.end(); ++current_block)
            {
                if (!(*current_block).is_hash_valid())
                    return false;

                // Connection of chains are valid?
                if (current_block != chain.begin())
                    if ((*current_block).get_previous_hash() != (*previous_block).get_hash())
                        return false;

                // All sinature are valid?
                if (!((*current_block).is_block_valid()))
                    return false;

                previous_block = current_block;
            }
            return true;
        }

        void mine_pending_transactions(unsigned long long int minning_reward_address)
        {
            if (reward_flag)
            {
                Transaction reward_transaction(0, minning_reward_address, minning_reward);
                (pending_transaction).push_back(reward_transaction);
            }
            block<Transaction, HashFunctionClass> new_block((chain.back().get_hash()), difficulty, pending_transaction, chain.size());
            chain.push_back(new_block);
            pending_transaction.clear();
        }

        void mine_pending_transactions()
        {
            block<Transaction, HashFunctionClass> new_block((chain.back().get_hash()), difficulty, pending_transaction, chain.size());
            chain.push_back(new_block);

            // Now liist is reset to null
            pending_transaction.clear();
        }

        void add_transaction(Transaction &temp)
        {
            // Signed?
            if (!temp.is_transaction_valid())
            {
                std::cout << "Invalid Transaction";
                exit(0);
            }
            (pending_transaction).push_back(temp);
        }

        // Iterated through all block which iterated though all transactions
        float get_balance(const unsigned long long int address)
        {
            float balance = 0;
            for (typename std::list<block<Transaction, HashFunctionClass>>::iterator current_block = chain.begin(); current_block != chain.end(); ++current_block)
                balance += (*current_block).get_balance(address);

            return balance;
        }

        // This class is made as iterface b/w user and block_chain
        // Wrapper over the operations of list<block>
        class iterator
        {
        private:
            // List iterator
            typename std::_List_iterator<ra::block<Transaction, HashFunctionClass>> block_ptr;

        public:
            iterator() {}
            iterator(std::_List_iterator<ra::block<Transaction, HashFunctionClass>> block_ptr)
            {
                this->block_ptr = block_ptr;
            }
            void set(std::_List_iterator<ra::block<Transaction, HashFunctionClass>> block_ptr)
            {
                this->block_ptr = block_ptr;
            }

            iterator operator=(const iterator &other)
            {
                block_ptr = other.block_ptr;
                return *this;
            }
            bool operator==(const iterator &other) { return (block_ptr == other.block_ptr); }
            bool operator!=(const iterator &other) { return (block_ptr != other.block_ptr); }
            block<Transaction, HashFunctionClass> &operator*() { return *(block_ptr); }
            iterator operator++(int)
            {
                iterator i = *this;
                block_ptr++;
                return i;
            }
            iterator &operator++()
            {
                block_ptr++;
                return *this;
            }
            const iterator *operator->() { return (block_ptr); }
        };

        const iterator begin()
        {
            iterator other(chain.begin());
            return other;
        }

        const iterator end()
        {
            iterator other(chain.end());
            return other;
        }

        block<Transaction, HashFunctionClass> &back()
        {
            return chain.back();
        }
        block<Transaction, HashFunctionClass> &front()
        {
            return chain.front();
        }
        int size()
        {
            return chain.size();
        }
    };
}
