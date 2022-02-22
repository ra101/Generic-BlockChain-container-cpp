# [namespace ra](https://ra101.github.io/namespace-ra/) :: BlockChain

### Aim: 
To create an <u>iterable</u> blockchain, which could work with an <u>extendable</u> transaction class. Here the link [GitHub](https://github.com/ra101/Generic-BlockChain-container-cpp) repo!

### Blockchain in Brief: 
- Blockchain is fundamentally a ledger maintaining records of transaction, This ledger is publicly distributed.
- Blockchain is a sort of linked-list of Blocks, A Block is a data-structure, that contains a set of transactions.
- Whenever a transaction happens, transferring party adds a reward for the "Miner".
- A "Miner" picks the set of transactions to be added in block till block-weight-limit (max allowed size of the block) is reached, then "Miner" does some heavy computation as proof of work and then sends the mined block in the world to add in each individual ledger. 
- These Transactions need not be monetary but could be anything that you want to make immutable.

### Scope of this Project: 
- Will not be doing any server-side operations.
- Will create a data-structure complete with custom error and iterators.
- Miners reward will be sent out by the admin.
- No block-weight-limit.
- Our Own Custom Proof of Work. 
- Extendable/Inheritable Transaction Class 


### Plan of Attack:
- We are going to use [RSA](https://ra101.github.io/RSA-cpp/) for our encryption needs.
- Extendable `base_transaction` class.
- Create `block` class with `Transaction` and `HashFunction` templates.
- Create a `blockchain` class with the same templates and custom iterators to make it iterable.

#### Setup 
Lets Setup the extra files!
##### RSA: Library will be used for signing and verification of transactions.

 Create a `includes` folder and add files from [Github](https://github.com/ra101/RSA-cpp)

<br>

##### `not_implemented_exception`: The exception that is thrown when a requested method or operation is not implemented.

```C++
class not_implemented_exception : public logic_error
{
public:
    not_implemented_exception(
        string function = __builtin_FUNCTION()
    ) : logic_error("`" + function + "` not implemented!"){};
};
```




#### `base_transaction<EncryptionAlgo>`
Here are the required things:

- Parametric Constructor with `from_addr`, `to_addr`, `transfer_amount`
- Hash Generator function (`generate_hash_input`)
- Override `string` Operator (for string type casting)
- Override `<<` Operator (for `cout`)
- Override `<` Operator (for comparison)
- Function to get balance based on address (`get_balance(addr)`)
- Function to sign transaction (`sign_transaction(key_pair)`)
- Function to validate transaction (`is_transaction_valid(verifying_func)`)

Every Function logic will be same, which is to throw `not_implemented_exception`, for example:

```C++
operator string() const {throw not_implemented_exception();}
```

#### `custom_transaction : public base_transaction<EncryptionAlgo>`

Private Members:
- `from_address` (Yours `public_key`)
- `to_address` (Others `public_key`)
- `transfer_amount`
- `timestamp`
- `signature` (Initially `0`, will be updated in `sign_transaction`)

Public Members:
- Parametric Constructor with `from_addr`, `to_addr`, `transfer_amount`

 ```C++
custom_transaction(from_adrr, to_addr, amount){
    from_address = from_addr;
    to_address = to_addr;
    transfer_amount = amount;
    timestamp = chrono::steady_clock::now();
    signature = 0;
}
```

- Hash Generator function (`generate_hash_input`)

 ```python
# Concatenation of all stringified private variables (except Signature)
# to_string(...) in C++
return (str(from_addr) + str(to_addr) + str(amount) + str(timestamp))
```


- Override `string` Operator (for string typecasting)

 ```C++
operator string() const
{
  string from_address_str = "\nfrom_address: " + to_string(from_address);
  string to_address_str = "\nto_address: " + to_string(to_address);
  string transfer_amount_str = "\ntransfer_amount: " + to_string(transfer_amount);
  string signature_str = "\nsignature: " + to_string(signature);
  return from_address_str + to_address_str + transfer_amount_str + signature_str;
}
```

- Override `<<` Operator (for `cout`)

 ```C++
friend ostream &operator<<(ostream &out, custom_transaction const &temp)
{
  // Using above string operator
  return out << string(temp);
}
```

- Override `<` Operator (for comparison)

```C++
bool operator<(const custom_transaction &other) const
{
  return generate_hash_input() < other.generate_hash_input();
}
```

- Function to get balance based on the address (`get_balance(addr)`)

 ```C++
float get_balance(addr) const
{
  if (addr == to_address)
    return transfer_amount;
  if (addr == from_address)
    return -transfer_amount;
  return 0;
}
```


- Function to sign transaction (`sign_transaction(key_pair)`)

 Here are some Pointers regarding signing
 - A Transaction is signed by the party making transaction therefore our `from_address` should match `public_key` in `key_pair`. Let us create an Error Regarding that:
 ```C++
class public_key_mismatch : public invalid_argument
{
public:
    public_key_mismatch(
        string addr, std::string pk) : invalid_argument("from_addr: '" + addr + "' do not match public_key: '" + pk + "'"){};
};
```
 - A Transaction is can be signed once, Let us create a Exception Regarding that:
 ```C++
class resign_transaction_exception : public logic_error
{
public:
    resign_transaction_exception(string transaction) : logic_error("Resigning Transaction!" + transaction){};
};
```
 - Signing of Transaction refers Signing the stringified Transaction class i.e. output of `generate_hash_input()`
 ```C++
template <typename EncryptionAlgo>
bool sign_transaction(EncryptionAlgo key_pair)
{
  if (from_address != key_pair.get_public_key())
    throw public_key_mismatch(to_string(from_address), to_string(key_pair.get_public_key()));
  if (signature != 0) // already signed
    throw resign_transaction_exception(*this);
  signature = key_pair.sign(generate_hash_input());
  return true;
}
```


- Function to validate transaction (`is_transaction_valid(verifying_func)`)

 ```C++
bool is_transaction_valid(bool (*verfication_function)(string, size_t, address)) const
{
  // reward, since reward is given by admin, there is no key pair
  if (from_address == 0)
    return true;
  return verfication_function(generate_hash_input(), signature, from_address);
}
```

#### `block <Transaction, HashFunctionClass>`

NOTES:
- A block typically contains the following:
  - ID
  - Previous Hash
  - timestamp
  - `nonce` ("number used once") a 32-bit, data computed in proof-of-work
  - A set of Transactions (Merkel Tree) 
  - Hash (stringification of all of the above)

-  `hash_function` is an instance of template class used for all hashing related needs (`std::hash` used in `main.cpp`)

- Proof of Work:
   - Calculating Hash in such a way that it becomes computationally intensive. A way to make it happen is:

 ```python
# Here `nonce` is found via brute force
# until HASH_PATTERNis matched.
while(1):
    hash_input = str(...) + str(nonce)
    if re.match(hash_function(hash_input), HASH_PATTERN):
        return nonce
    nonce += 1
```

 - In **BitCoin**:
   - hash_function: `sha256`
   - HASH_PATTERN: Starting with `n` zeros, higher the `n`, harder to compute.

 - In `namespace::ra` :
   - hash_function: Template (`std::hash<string>` for reference)
   - HASH_PATTERN: hash length must be `15/n`, lesser the n, harder to compute.



#### `block_chain <Transaction, HashFunctionClass>`

Well as the name states it is a chain of blocks, So Naturally, It will contain the same Template Arguments as `Block`

Private Members:
- `list<BlockType> chain` (A list of blocks)
- `list<Trasaction> pending_transaction` (Temperaroy Buffer before mining (_create a block and adding it to list_))
- `difficulty` (to be passed in `block` class)
- `minning_reward` (If exists, will add a new transaction from admin before mining block)
- `verfication_function` (to verify block or transaction itself)

Public Members:
- Iterator Class (To make iteration easy)

 - Private:
   - `std::_List_iterator<block_type>` block_ptr, 
 - Public:
   - Override `==`, `!=`, `++`, `&++`, `--`, `&--`, for example:

  ```C++
iterator operator--(int)
{
    iterator _temp = *this;
    m_block_ptr--;
    return _temp;
}
```

- Override the following to work with iterator class

 ```C++
const iterator begin() { return chain.begin(); }
const iterator end() { return chain.end(); }
block_type &back() { return chain.back(); }
block_type &front() { return chain.front(); }
int size() { return chain.size(); }
``` 


- `block_chain(diffuculty, verfication_function, minning_reward)` Constructor:
  - Update all the private variable by this constructor
  - Add A Genesis Block to chain! 


- get_balance(): 
  - `float balance = 0`
  - Iterator over the `block_chai`n from `begin()` to `end()` using iterator class and add it to `balance`
  -  return `balance`


- `add_transaction(temp)`
  - check `temp` using `is_transaction_valid` by passing in `verfication_function`
  - add to `pending_transaction` else Throw Error


- `mine_pending_transactions(minning_reward_address)`
  - if `minning_reward` exsits, add a reward `transaction` to `minning_reward_address`, else continue
  - create a block with `pending_transaction`, and push it to chain.
  - clear `pending_transaction`.

- `is_chain_valid()`
  - Iterator over the `chain`, check the validity of current block using `is_block_valid` by passing in `verfication_function`
  - check if `previous_hash` of current block matches `hash` of the previous block
