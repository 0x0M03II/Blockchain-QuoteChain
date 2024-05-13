#include "quotechain.h"

int main(void) {
    block* head = nullptr;
    block* tail = nullptr;

    // Genesis Block
    QuoteChain chain("quotes.txt");

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