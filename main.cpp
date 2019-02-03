//#include <iostream> //already included in blockCchian class
#include "BlockChain.h"
#include "Transaction.h"
// #include "includes/RSA.h" //already Included in Transaction Class

int main()
{
  ra::RSAKeyPair k1("127.0.0.1:80");
  ra::RSAKeyPair k2("189.0.2.47:52");

  ra::BlockChain<ra::Transaction,std::hash<std::string> > raCoin(5,50); //5,6,7 works best
  //ra::BlockChain<ra::Transaction,std::hash<std::string> > raCoin(6); //5,6,7 works best

  ra::Transaction t1(k1.getPublicKey(),k2.getPublicKey(),50);
  t1.signTransaction(k1); 
  raCoin.addTransaction(t1);

  raCoin.minePendingTransactions(k2.getPublicKey());

  for(ra::BlockChain<ra::Transaction,std::hash<std::string>>::iterator abc = raCoin.begin();abc!=raCoin.end();abc++)
  {
    std::cout<<*abc;
  }
  std::cout<<"\n\nIs Chain Valid: "<<raCoin.isChainValid();

  std::cout<<"\n\n k2 balance : "<<raCoin.getBalance(k2.getPublicKey());
  return 0;
}