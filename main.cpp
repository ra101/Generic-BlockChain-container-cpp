#include <iostream>
#include <sstream>
#include <chrono>

#include "block_chain.hpp"
#include "transaction.hpp"
#include "exceptions.hpp"

#include "includes/rsa.hpp"

class custom_transaction : public ra::base_transaction
{
private:
  unsigned long long int from_address; // your public key
  unsigned long long int to_address;   // ohers public key
  float transfer_amount;
  std::chrono::steady_clock::time_point timestamp;
  std::size_t signature;

public:
  custom_transaction(const unsigned long long int from_address, const unsigned long long int to_address, const float &transfer_amount)
  {
    this->from_address = from_address;
    this->to_address = to_address;
    this->transfer_amount = transfer_amount;
    timestamp = std::chrono::steady_clock::now();
    signature = 0;
  }

  std::string generate_hash_input() const
  {
    std::string hash_input = std::to_string((std::chrono::duration_cast<std::chrono::microseconds>(this->timestamp.time_since_epoch()).count()));
    hash_input += std::to_string(this->from_address) + std::to_string(this->to_address) + std::to_string(this->transfer_amount);
    return hash_input;
  }
  bool operator<(const custom_transaction &other) const
  {
    return generate_hash_input() < other.generate_hash_input();
  }
  operator std::string() const
  {

    std::string from_address_str = "\nfrom_address: " + std::to_string(from_address);
    std::string to_address_str = "\nto_address: " + std::to_string(to_address);
    std::string transfer_amount_str = "\ntransfer_amount: " + std::to_string(transfer_amount);
    std::string signature_str = "\nsignature: " + std::to_string(signature);

    return from_address_str + to_address_str + transfer_amount_str + signature_str;
  }

  friend std::ostream &operator<<(std::ostream &out, custom_transaction const &temp)
  {
    return out << std::string(temp);
  }

  float get_balance(unsigned long long int address) const
  {
    if (address == this->to_address)
      return transfer_amount;

    if (address == this->from_address)
      return (-transfer_amount);

    return 0;
  }

  template <typename EncryptionAlgo> // makes sense for multiple transactions
  bool sign_transaction(EncryptionAlgo key_pair)
  {
    if (from_address != key_pair.get_public_key())
    {
      throw ra::public_key_mismatch(std::to_string(from_address), std::to_string(key_pair.get_public_key()));
    }

    // already signed
    if (signature != 0)
      throw ra::resign_transaction_exception(*this);

    signature = key_pair.sign(generate_hash_input());
    return true;
  }

  bool is_transaction_valid(bool (*verfication_function)(std::string, std::size_t, unsigned long long int)) const
  {
    // reward
    if (from_address == 0)
      return true;

    return verfication_function(generate_hash_input(), signature, from_address);
  }
};

int main()
{
  ra::rsa_key_pair k1("127.0.0.1:80");
  ra::rsa_key_pair k2("189.0.2.47:52");

  ra::block_chain<custom_transaction, std::hash<std::string>> ra_coin(MED_DIFFICULTY, ra::verify, 50);
  // ra::block_chain<custom_transaction, std::hash<std::string>> ra_coin(MAX_DIFFICULTY, ra::verify);

  custom_transaction t1(k1.get_public_key(), k2.get_public_key(), 1000);

  /*
    Update the line below in the following manner to get the following exceptions
    comment it -> Invalid Transaction Error
    duplicate it -> Resign Exception
    replace k1 with k2 -> Mismatch
  */
  t1.sign_transaction<ra::rsa_key_pair>(k1);

  ra_coin.add_transaction(t1);

  ra_coin.mine_pending_transactions(k1.get_public_key());

  for (ra::block_chain<custom_transaction, std::hash<std::string>>::iterator abc = ra_coin.begin(); abc != ra_coin.end(); abc++)
  {
    std::cout << *abc;
  }
  std::cout << "\n\nIs Chain Valid: " << ra_coin.is_chain_valid();

  std::cout << "\n\nk2 balance : " << ra_coin.get_balance(k2.get_public_key());
  std::cout << "\nk1 balance : " << ra_coin.get_balance(k1.get_public_key());
  return 0;
}
