# Generic-block_chain-container-cpp
ref1a: https://www.youtube.com/playlist?list=PLzvRQMJ9HDiTqZmbtFisdXFxul5k0F-Q4<br>
ref1b: https://github.com/Savjee/SavjeeCoin<br>
ref2: https://www.youtube.com/watch?v=JR4_RBb8A9Q<br>
rsa.h: https://github.com/60d-RA-3Eh/RSA-cpp<br>
prime.h: https://github.com/60d-RA-3Eh/Random-Prime-Number-Generator-Engine-cpp<br>
## Generic block_chain container using rsa and std::hash as template input
just go thorugh main.cpp for brief understanding.<br>
ra::Transaction(from_address,to_address,transfer_amount);  //transaction class includes RSA<br>
ra::block_chain<Transaction Class,hashFunctionClass> ra_coin(5,50); //5,6,7 works best<br>
ra::block_chain<a,b>::iterator itr;<br>
for proof of work, it do not follow the standard method , instead it follows a c++ method of time and processing consumption.<br>
