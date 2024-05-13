#include <iostream>
#include "quotechain.h"

int main(void) {
    block* head = nullptr;
    block* tail = nullptr;

    // Genesis Block
    QuoteChain chain("quotes.txt");

    // Intro Message
    std::cout << "================================" << std::endl;
    std::cout << "========== QuoteChain ==========" << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << std::endl;
    
    std::cout << "================================" << std::endl;
    std::cout << "Hashing:  Hash(Bn) = Hash(Hash(Bn - 1) || nonce || quote)" << std::endl;
    std::cout << "Proof-of-Work:  First 5 bits = 0" << std::endl;
    std::cout << "================================" << std::endl;
    std::cout << std::endl;

    std::string quoteGen = chain.GenerateQuote();
    std::vector<uint8_t> nonceGen = chain.GenerateNonce();
    chain.AddBlock(head, tail, quoteGen, nonceGen);
    
    // first block
    std::string quoteOne = chain.GenerateQuote();
    std::vector<uint8_t> nonceOne = chain.GenerateNonce();
    chain.AddBlock(head, tail, quoteOne, nonceOne);

    // second block
    std::string quoteTwo = chain.GenerateQuote();
    std::vector<uint8_t> nonceTwo = chain.GenerateNonce();
    chain.AddBlock(head, tail, quoteTwo, nonceTwo);

    // third block
    std::string quoteThree = chain.GenerateQuote();
    std::vector<uint8_t> nonceThree = chain.GenerateNonce();
    chain.AddBlock(head, tail, quoteThree, nonceThree);

    // print chain
    chain.PrintChain(head);
    return 0;
}