// #include <iostream>  //already add to blockChain
// #include <chrono>    //already add to blockChain
#include "includes/RSA.h" 
#define keyClass RSAKeyPair     //we can use template as well

namespace ra
{
class Transaction
{
private:
    unsigned long long int fromAddress; //your public key
    unsigned long long int toAddress;   //ohers public key
    float transferAmount;  
    std::chrono::steady_clock::time_point timeStamp;
    std::size_t signature;
public:
    Transaction(const unsigned long long int fromAddress, const unsigned long long int toAddress, const float &transferAmount);
    std::string generateHashInput();    //converts to string and concatenates them
    friend std::ostream& operator << (std::ostream&,  Transaction const &); //to print transacttion
    float getBalance(const unsigned long long int); //makes sense for multiple transactions
    bool signTransaction(keyClass keyPair);     //hash of input happen during signing 
    bool isTransactionValid();      //uses verify of rsa
};

Transaction::Transaction(const unsigned long long int fromAddress,const unsigned long long int toAddress, const float& transferAmount)
{
    this->fromAddress=fromAddress;
    this->toAddress=toAddress;
    this->transferAmount=transferAmount;
    this->timeStamp=std::chrono::steady_clock::now();
    this->signature=0;  
}

std::string Transaction::generateHashInput()
{
    std::string hashInput = std::to_string((std::chrono::duration_cast<std::chrono::microseconds>(this->timeStamp.time_since_epoch()).count()));
    hashInput += std::to_string(this->fromAddress) + std::to_string(this->toAddress) + std::to_string(this->transferAmount);
    return hashInput;
}
 
std::ostream& operator << (std::ostream& out,  Transaction const & temp)
{
    out << "\nfromAddress: " << temp.fromAddress;
    out << "\ntoAddress: " << temp.toAddress;
    out << "\ntransferAmount: " << temp.transferAmount;
    out << "\nSignature: " << temp.signature;
    return out;
}

float Transaction::getBalance(unsigned long long int address)
{
    if(address==this->toAddress)
    {
        return transferAmount;
    }
   if(address==this->fromAddress)
    {
        return (-transferAmount);
    }
    return 0;
}

bool Transaction::signTransaction(keyClass keyPair)
{
    if(fromAddress!=keyPair.getPublicKey())
    {
         std::cout<<"address and key donot match";
        exit(0);
    }
    if(signature!=0)    //already signed
    {
        return false;
    }
    this->signature = keyPair.sign(this->generateHashInput());
    return true;
}

bool Transaction::isTransactionValid()
{
    if(fromAddress==0)  //reward
    {
        return true;
    }
    if(signature==0)    //unsigned and not reward
    {
        return false;
    }
    return verify(this->generateHashInput(),this->signature,this->fromAddress);
}

}