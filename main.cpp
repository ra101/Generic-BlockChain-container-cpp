#include <iostream>

#include "block_chain.hpp"
#include "transaction.hpp"

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
  friend std::ostream &operator<<(std::ostream &out, custom_transaction const &temp)
  {
    out << "\nfrom_address: " << temp.from_address;
    out << "\nto_address: " << temp.to_address;
    out << "\ntransfer_amount: " << temp.transfer_amount;
    out << "\nsignature: " << temp.signature;
    return out;
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
      std::cout << "address and key donot match";
      exit(0);
    }

    // already signed
    if (signature != 0)
      return false;

    signature = key_pair.sign(generate_hash_input());
    return true;
  }

  template <typename Message> // makes sense for multiple transactions
  bool is_transaction_valid(bool (*verfication_function)(Message, std::size_t, unsigned long long int)) const
  {
    // reward
    if (from_address == 0)
      return true;

    // unsigned and not reward
    if (signature == 0)
      return false;

    return verfication_function(generate_hash_input(), signature, from_address);
  }
};

int main()
{
  ra::rsa_key_pair k1("127.0.0.1:80");
  ra::rsa_key_pair k2("189.0.2.47:52");

  ra::block_chain<custom_transaction, std::hash<std::string>, std::string> ra_coin(MED_DIFFICULTY, 50, ra::verify);
  // ra::block_chain<custom_transaction, std::hash<std::string>> ra_coin(MAX_DIFFICULTY);

  custom_transaction t1(k1.get_public_key(), k2.get_public_key(), 1000);
  t1.sign_transaction<ra::rsa_key_pair>(k1);
  ra_coin.add_transaction(t1);

  ra_coin.mine_pending_transactions(k2.get_public_key());

  for (ra::block_chain<custom_transaction, std::hash<std::string>, std::string>::iterator abc = ra_coin.begin(); abc != ra_coin.end(); abc++)
  {
    std::cout << *abc;
  }
  std::cout << "\n\nIs Chain Valid: " << ra_coin.is_chain_valid();

  std::cout << "\n\nk2 balance : " << ra_coin.get_balance(k2.get_public_key());
  return 0;
}
