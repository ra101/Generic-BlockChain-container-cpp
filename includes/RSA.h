#include "prime.h"
#include <random>   // for template input of prime engine
//#include <chrono>  //for time seed //already added to blockchain.h

namespace ra{
#define aBigNo 100000000000
#define padLen 10

class RSAKeyPair
{
private:
    unsigned long long int privateKey;
    unsigned long long int publicKey;
    int getE(int);
    int getK(int,int);
    bool areKeysValid();
    void createKey(unsigned long);
public:
    RSAKeyPair();
    RSAKeyPair(std::string);
    std::size_t decript(unsigned);
    std::size_t decriptWithPadding(std::string);
    unsigned long long int getPublicKey();
    unsigned long long int getPrivateKey();
    std::size_t sign(std::string message); 
};
//below function are nor the part of class neither friend function
std::string padding(std::string);
std::size_t encript(std::size_t,unsigned long long int);
std::string encriptWithPadding(std::size_t,unsigned long long int);
bool verify(std::string,std::size_t,unsigned long long int);

RSAKeyPair::RSAKeyPair()
{
    std::chrono::steady_clock::time_point timeStamp = std::chrono::steady_clock::now();
    unsigned long time_seed = ((std::chrono::duration_cast<std::chrono::nanoseconds>(timeStamp.time_since_epoch()).count()));
    createKey(time_seed);
}

RSAKeyPair::RSAKeyPair(std::string userId)
{
    std::hash<std::string> hashFuntion;
    unsigned long seed = hashFuntion(userId);
    createKey(seed);
}

void RSAKeyPair::createKey(unsigned long seed)
{
    random_prime_engine<std::default_random_engine> eng(seed);
    do
    {
    int p1=eng();   //two random prime int
    int p2=eng();
    int n=p1*p2;
    int phin = n-p1-p2+1;               //(p1-1)*(p2-1)   
    int e=getE(phin);
    int k=getK(phin,e);
    int d= (k*phin +1)/e;
    publicKey = n*aBigNo + e;       //multipplication to aBigNo is way to concatenate n and e or d
    privateKey = n*aBigNo + d;
    }
    while(!areKeysValid());
}

int RSAKeyPair::getE(int phin)      //small odd no. which doesn't share factor with phin
{
    for(int e=3;e<phin;e+=2)
    {
        if(phin%e!=0)
        {
            return e;
        }
    }
}


int RSAKeyPair::getK(int phin,int e)     //such that k*phin +1 / e (=d) must be integer
{
    for(int k=1;k<phin;k+=1)
    {
        if((k*phin + 1)%e==0)
        {
            return k;
        }
    }
}

bool RSAKeyPair::areKeysValid()
{
    return((this->publicKey/aBigNo)==(this->privateKey/aBigNo)); 
    // 'n' of both no. are same
}

unsigned long long int RSAKeyPair::getPublicKey()
{
    return  (this->publicKey);
}

unsigned long long int RSAKeyPair::getPrivateKey()
{
    return  (this->privateKey);
}


std::size_t encript(std::size_t digest,  unsigned long long int othersPublicKey)
{
    int n = othersPublicKey/aBigNo; //retrive n
    int e =othersPublicKey % aBigNo;    //retrive e
    long long int res = 1;      // Initialize result
    long long int x = digest;
    x = x % n; // Update x if it is more than or
                // equal to n
    while (e > 0)
    {
    // If e is odd, multiply x with result
        if (e & 1)
            res = (res*x) % n;
        // e must be even now
        e = e>>1;   // e = e/2
        x = (x*x) % n;
    }
    return res;
}

std::size_t RSAKeyPair::decript(unsigned encriptedData)
{
    int n = this->privateKey/aBigNo;    //retrive n
    int d =this->privateKey % aBigNo;   //retrive d
    unsigned long long int res = 1;
    unsigned long long int x =encriptedData;
    x = x % n;  
    while (d > 0)       //exact simmilar encript
    {
        if (d & 1)
            res = (res*x) % n;
        d = d>>1;
        x = (x*x) % n;
    }
    return res;
}

std::string padding(std::string outputNo)
{
    while(outputNo.length()<padLen)
    {
        outputNo = '0' + outputNo; //add 0 infront of no.
    }
    return outputNo;
}

std::string encriptWithPadding(std::size_t digest,unsigned long long int othersPublicKey)
{
    std::string output;
    while(digest!=0)
    {
        //encript one digit at a time and pad it
        output = padding(std::to_string(encript((digest%10),othersPublicKey))) + output;
        digest /=10;
    }
    return output;
}

std::size_t RSAKeyPair::decriptWithPadding(std::string encriptedData)
{
    int digit=0;
    std::size_t res=0;
    int i,n=encriptedData.length();
    for(i=0;i<n;digit=0)
    {
        while(i<n-1 && encriptedData.at(i) == '0') //let all zeros pass and n-1 'cuz of i+=1
        {
            i+=1;
            if(i%padLen==0)
            {
            //if we pass a padlength of '0's (encription of zero is zero)
                res = res*10;
            }
        }
        while(i%padLen!=0)  //change loop after each padlength
        {
            digit = digit*10 + (encriptedData.at(i) - '0');  //create digit
            i+=1;
        }
        res = res*10 + decript(digit); 
    }
    return res;
}

std::size_t RSAKeyPair::sign(std::string message)
{
    std::hash<std::string> hashFuntion;
    unsigned short int hashOutput =  hashFuntion(message); //short int 'cuz of sizeLimit(x*x) of decription
    return  (decript(hashOutput)); //signing is nothing but decript of hashed msg
} 

bool verify(std::string message,std::size_t signature,unsigned long long int othersPublicKey)
    {
    std::hash<std::string> hashFuntion;
    short int hashOutput =  hashFuntion(message);
    short int res = encript(signature,othersPublicKey);
    return (res==hashOutput);
    }
}