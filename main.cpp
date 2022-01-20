#include <iostream>

#include "block_chain.hpp"
#include "transaction.hpp"
#include "includes/rsa.hpp"

int main()
{
  ra::rsa_key_pair k1("127.0.0.1:80");
  ra::rsa_key_pair k2("189.0.2.47:52");

  ra::block_chain<ra::transaction, std::hash<std::string>, std::string> ra_coin(MIN_DIFFICULTY, 50, ra::verify);
  // ra::block_chain<ra::Transaction,std::hash<std::string> > ra_coin(MAX_DIFFICULTY);

  ra::transaction t1(k1.get_public_key(), k2.get_public_key(), 50);
  t1.sign_transaction<ra::rsa_key_pair>(k1);
  ra_coin.add_transaction(t1);

  ra_coin.mine_pending_transactions(k2.get_public_key());

  for (ra::block_chain<ra::transaction, std::hash<std::string>, std::string>::iterator abc = ra_coin.begin(); abc != ra_coin.end(); abc++)
  {
    std::cout << *abc;
  }
  std::cout << "\n\nIs Chain Valid: " << ra_coin.is_chain_valid();

  std::cout << "\n\nk2 balance : " << ra_coin.get_balance(k2.get_public_key());
  return 0;
}
