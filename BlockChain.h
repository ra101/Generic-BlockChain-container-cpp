#include <iostream>
#include <list>
#include "Block.h"
//#include "Transaction.h" //included in Block.h
// each class is independed of other there exits no herachy yet data can't be manipulated due to checks
namespace ra{

template <class Transaction,class HashFunctionClass>
 //hashFunctionClass has no funtionality in this header, but it has to be passed into Block constructor
class BlockChain
{
private:
    std::list< ra::Block<Transaction,HashFunctionClass> > chain;
    int difficulty;
    bool rewardflag;    //it exists for a non reward based Blockchain
    float minningReward;
    std::list<Transaction> pendingTransaction; 
public:
    BlockChain(int difficulty,float minningReward)
    {
        this->difficulty=difficulty;
        this->minningReward=minningReward;
        chain.push_back(*(new Block<Transaction,HashFunctionClass>(0,this->difficulty,this->pendingTransaction,chain.size())));
        rewardflag =true;
    }
    
    BlockChain(int difficulty)
    {
        this->difficulty=difficulty;
        chain.push_back(*(new Block<Transaction,HashFunctionClass>(0,this->difficulty,this->pendingTransaction,chain.size())));
        rewardflag =false;
    }
    bool isChainValid()
    {    
    for (typename std::list< Block<Transaction,HashFunctionClass> >::iterator previousBlock,currentBlock = chain.begin(); currentBlock != chain.end(); ++currentBlock)
    {
        if(!(*currentBlock).isHashValid())
        {
            return false;
        }
        if(currentBlock!=chain.begin()) //connection of chains are valid?
        {
            if((*currentBlock).getPreviousHash()!=(*previousBlock).getHash())
            {
                return false;
            }
        }
        if(!((*currentBlock).isBlockValid()))   //all sinature are valid?
        {
            return  false;
        }
        previousBlock=currentBlock;
    }
    return true;
    }

    void minePendingTransactions(unsigned long long int minningRewardAddress)
    {
    if(rewardflag)
    {
        Transaction rewardTransaction(0,minningRewardAddress,this->minningReward);
        (this->pendingTransaction).push_back(rewardTransaction);
    }
    Block<Transaction,HashFunctionClass> newBlock((chain.back().getHash()),this->difficulty,this->pendingTransaction,chain.size());
    chain.push_back(newBlock);  
    pendingTransaction.clear();
    }

    void minePendingTransactions()
    {
    Block<Transaction,HashFunctionClass> newBlock((chain.back().getHash()),this->difficulty,this->pendingTransaction,chain.size());
    chain.push_back(newBlock);  
    pendingTransaction.clear(); //now liist is reset to null
    }

    void addTransaction(Transaction& temp)
    {
        if(!temp.isTransactionValid())  //signed?
        {
            std::cout<<"InValid Transaction";
            exit(0);
        }
        (this->pendingTransaction).push_back(temp);
    }
    float getBalance(const unsigned long long int address)  //iterated through all block which iterated though all transactions
    {
        float balance=0;
        for (typename std::list<Block<Transaction,HashFunctionClass> >::iterator currentBlock = chain.begin(); currentBlock != chain.end(); ++currentBlock)
        {  
        balance +=  (*currentBlock).getBalance(address);
        }
        return balance;
    }

//this class is made as iterface b/w user and blockchain
    class iterator  //cover over the operation of list<block>
    {
    private:
        typename std::_List_iterator<ra::Block<Transaction,HashFunctionClass> > block_ptr; //list iterator
    public:
    iterator() {}
    iterator(std::_List_iterator<ra::Block<Transaction,HashFunctionClass> > block_ptr) {
        this->block_ptr = block_ptr;
    }
    void set(std::_List_iterator<ra::Block<Transaction,HashFunctionClass> > block_ptr) {
        this->block_ptr = block_ptr;
    }

    iterator operator=(const iterator& other) { this->block_ptr = other.block_ptr;return *this;}
    bool operator==(const iterator& other) { return (this->block_ptr == other.block_ptr);}
    bool operator!=(const iterator& other) { return (this->block_ptr != other.block_ptr);}
    Block<Transaction,HashFunctionClass>& operator*() { return *(this->block_ptr);}
    iterator operator++(int) { iterator i = *this; (this->block_ptr)++; return i; }
    iterator& operator++() { (this->block_ptr)++; return *this; }
    const iterator* operator->() { return (this->block_ptr); }
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

    Block<Transaction,HashFunctionClass>& back()
    {
        return chain.back();    
    }
    Block<Transaction,HashFunctionClass>& front()
    {
        return chain.front();    
    }
    int size()
    {
        return chain.size();    
    }
    };
}