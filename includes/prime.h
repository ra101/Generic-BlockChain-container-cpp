#include <fstream>              //for file-handling
namespace ra{
#define primeDBSize 1000        //different form "Random-Prime-Number-Generator-Engine-cpp" repos

template<typename randomEngine>
class random_prime_engine
{
    randomEngine eng;   //this engine takes random engines as template input
    int minPrime;
    int maxPrime;
public:
    random_prime_engine();
    random_prime_engine(unsigned long);
    int operator()();
    template<class engineTemplate>
    friend std::stringstream & operator << (std::stringstream&, const random_prime_engine<engineTemplate>&);
    template<class engineTemplate>
    friend std::stringstream & operator >> (std::stringstream&, random_prime_engine<engineTemplate>&);
    int min();
    int max();
};

template<typename randomEngine>
random_prime_engine<randomEngine>::random_prime_engine()
{
    //lazy intiallisation
    minPrime=0;  
    maxPrime=0;
}

template<typename randomEngine>
random_prime_engine<randomEngine>::random_prime_engine(unsigned long rand_seed)
{
    //lazy intiallisation
    minPrime=0;  
    maxPrime=0;
    this->eng.seed(rand_seed);
}

template<typename randomEngine>
int random_prime_engine<randomEngine>::operator()()
{
    int idx = eng() % primeDBSize; //to get a index within file
    int retrunValue;
    std::ifstream file;
    file.open("includes/primeDB");
    for(int i=0;i<=idx;i++)
    {
        file>>retrunValue; //update returnvalue till it gets the value at index
    }
    file.close();
    return(retrunValue);
}

template<typename randomEngine>
int random_prime_engine<randomEngine>::min()
{
    if(!minPrime) //if min==0 , i.e. first time intiallization
    {
        std::ifstream file;
        file.open("includes/primeDB");
        file>>this->minPrime;   //get int at the start of file
        file.close();
    }
    return this->minPrime;
}

template<typename randomEngine>
int random_prime_engine<randomEngine>::max()
{
    if(!maxPrime) //if max==0 , i.e. first time intiallization
    {
        std::ifstream file;
        file.open("includes/primeDB");
        while(file)
        {
        file>>this->maxPrime; //get int at the end of file
        }
        file.close();
    }
    return this->maxPrime;
}

template<class engineTemplate>
std::stringstream & operator << (std::stringstream &out, const random_prime_engine<engineTemplate> &primeClass)
{ 
    out << primeClass.eng; //save the state
    return out; 
} 

template<class engineTemplate>
std::stringstream & operator >> (std::stringstream &in, random_prime_engine<engineTemplate> &primeClass)
{ 
    in >> primeClass.eng; //retrun the state
    return in; 
} 
}