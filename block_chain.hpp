#include <iostream>
#include <list>
#include <set>

#include "block.hpp"
#include "exceptions.hpp"
// Each class is independed of other there exits no herachy yet data can't be manipulated due to checks

namespace ra
{

    // HashFunctionClass has no funtionality in this header, but it has to be passed into block constructor
    template <class Transaction, class HashFunctionClass>
    class block_chain
    {
    private:
        using block_type = block<Transaction, HashFunctionClass>;

        std::list<block_type> chain;
        std::set<Transaction> pending_transaction;

        int difficulty;
        float minning_reward;

        // It exists for a non reward based block_chain
        bool reward_flag;

        // Transaction will use this verfication function
        bool (*verfication_function)(std::string, std::size_t, unsigned long long int);

    public:
        block_chain(int difficulty, float minning_reward, bool (*verfication_function)(std::string, std::size_t, unsigned long long int))
        {
            this->difficulty = difficulty;
            this->minning_reward = minning_reward;
            this->verfication_function = verfication_function;
            chain.push_back(*(new block_type(0, difficulty, pending_transaction, size())));
            reward_flag = true;
        }

        block_chain(int difficulty)
        {
            this->difficulty = difficulty;
            chain.push_back(*(new block_type(0, difficulty, pending_transaction, size())));
            reward_flag = false;
        }
        bool is_chain_valid()
        {
            for (iterator previous_block, current_block = begin(); current_block != end(); ++current_block)
            {
                if (!(*current_block).is_hash_valid())
                    return false;

                // Connection of chains are valid?
                if (current_block != begin())
                    if ((*current_block).get_previous_hash() != (*previous_block).get_hash())
                        return false;

                // All sinature are valid?
                if (!((*current_block).is_block_valid(verfication_function)))
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
                pending_transaction.insert(reward_transaction);
            }
            block_type new_block((chain.back().get_hash()), difficulty, pending_transaction, size());
            chain.push_back(new_block);
            pending_transaction.clear();
        }

        void mine_pending_transactions()
        {
            block_type new_block((chain.back().get_hash()), difficulty, pending_transaction, size());
            chain.push_back(new_block);

            // Now liist is reset to null
            pending_transaction.clear();
        }

        void add_transaction(Transaction &temp)
        {
            // Signed?
            if (!temp.is_transaction_valid(verfication_function))
            {
                throw invalid_transaction_error(temp);
            }
            pending_transaction.insert(temp);
        }

        // Iterated through all block which iterated though all transactions
        float get_balance(const unsigned long long int address)
        {
            float balance = 0;
            for (iterator current_block = begin(); current_block != end(); ++current_block)
                balance += (*current_block).get_balance(address);

            return balance;
        }

        // This class is made as iterface b/w user and block_chain
        // Wrapper over the operations of list<block>
        class iterator
        {
        private:
            // List iterator
            using pointer_type = std::_List_iterator<block_type>;
            pointer_type m_block_ptr;

        public:
            iterator() {}
            iterator(pointer_type m_block_ptr) { this->m_block_ptr = m_block_ptr; }
            void set(pointer_type m_block_ptr) { this->m_block_ptr = m_block_ptr; }
            bool operator==(const iterator &other) { return m_block_ptr == other.m_block_ptr; }
            bool operator!=(const iterator &other) { return m_block_ptr != other.m_block_ptr; }
            block_type &operator*() { return *m_block_ptr; }
            const iterator *operator->() { return m_block_ptr; }

            iterator operator=(const iterator &other)
            {
                m_block_ptr = other.m_block_ptr;
                return *this;
            }
            iterator operator++(int)
            {
                iterator _temp = *this;
                m_block_ptr++;
                return _temp;
            }
            iterator &operator++()
            {
                m_block_ptr++;
                return *this;
            }
            iterator operator--(int)
            {
                iterator _temp = *this;
                m_block_ptr--;
                return _temp;
            }
            iterator &operator--()
            {
                m_block_ptr--;
                return *this;
            }
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
        block_type &back() { return chain.back(); }
        block_type &front() { return chain.front(); }
        int size() { return chain.size(); }
    };
}
