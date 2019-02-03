#include <chrono>

namespace ra{

template <class Transaction,class HashFunctionClass>   
class Block
{
private:
    int id;
    std::chrono::steady_clock::time_point timeStamp;    //block and transaction both require timestamp
    std::size_t previousHash;
    std::size_t hash;
    HashFunctionClass hashFunction; //template input as here std::hash is used but u can make sha256 and the inetegrate them
    int nonce;  //random data    
    std::list<Transaction> transactionList;   //pending transactin will become tranctionlist here 
    int findHashLength(int difficulty)  //for proof of work
    {
        int factor=1;
        for(int i=0;i<difficulty;i++)
        {
            factor *=10;
        }
        return (10000000000/factor); //hash functor produces a number of 10 or 9 digits
    }

    std::string generateHashInput() //converion to string and concatenation of data
    {
        std::string hashInput = std::to_string((std::chrono::duration_cast<std::chrono::microseconds>(this->timeStamp.time_since_epoch()).count()));
        for(typename std::list<Transaction>::iterator tranElem=transactionList.begin();tranElem != transactionList.end();tranElem++)
        {
            hashInput += (*tranElem).generateHashInput();    
        }
        hashInput += std::to_string(this->previousHash);
        return hashInput;
    }
public:
    Block(std::size_t previousHash,int difficulty,std::list<Transaction> transactionList,int id)
    {
        this->previousHash=previousHash;
        this->timeStamp = std::chrono::steady_clock::now();
        this->transactionList=transactionList;
        this->nonce=0;
        this->hash=mineBlock(generateHashInput(),findHashLength(difficulty));
        this->id=id;
    }
    std::size_t mineBlock(std::string hashInput,int hashLength)
    {   

        std::size_t hashOutput = hashFunction(hashInput+std::to_string(this->nonce));
        while(hashOutput/hashLength != 0)   //own time of proof of work
        {
            this->nonce +=1;
            hashOutput=hashFunction(hashInput+std::to_string(this->nonce));
        }
        return hashOutput;
    }
    std::size_t getHash()
    {
        return (this->hash);
    }

    std::size_t getPreviousHash()
    {
        return (this->previousHash);
    }

    bool isHashValid()  //recalculates hash and matches to current hash
    {
        std::string hashInput = generateHashInput();
        hashInput += std::to_string(this->nonce);
        return (this->hash == hashFunction(hashInput));
    }
    bool isBlockValid()
    {

    for(typename std::list<Transaction>::iterator tranElem=this->transactionList.begin();tranElem != this->transactionList.end();tranElem++)
        {
        if(!((*tranElem).isTransactionValid()))
        {
            return false;
        }
        }
        return true;
    }


    float getBalance(const unsigned long long int address)  //traverse thorugh all transaction
    {
        float balance=0;
        for(typename std::list<Transaction>::iterator tranElem=transactionList.begin();tranElem != transactionList.end();tranElem++)
        {
            balance += (*tranElem).getBalance(address);
        }
        return balance;
    }

    //modifiable.
    friend std::ostream& operator << (std::ostream& out,  Block<Transaction,HashFunctionClass> const & temp)
    {
    int i=1;
    if(temp.id==0)
    {
        out<<"...\nGenesis Block\n";
    }
    else
    {
        out<<"\n\n...\nBlock No. "<<temp.id;
    }
    out << "\n\nNo of Entries: " << temp.transactionList.size()  << "\nHash: " << temp.hash << "\nPrevious Hash: " << temp.previousHash;
        for(typename std::list<Transaction>::const_iterator tranElem=(temp.transactionList).begin();tranElem != temp.transactionList.end();tranElem++,i++)
        {
        out << "\n\nEntry no: " <<i<<(*tranElem);
        }
    return out;
}


};

}