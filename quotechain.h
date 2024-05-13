#ifndef QUOTECHAIN_H
#define QUOTECHAIN_H

#include <string>
#include <vector>
#include <sstream>

typedef struct _block {
    std::string quote;
    std::string blockhash;
    std::vector<uint8_t> nonce;
    struct _block* next;

    _block() : nonce(256, 0), next(nullptr) {}
} block;

class QuoteChain {

public:
    void CreateBlock();
    QuoteChain(std::string fp);
    std::string GenerateQuote();
    void PrintChain(block* head);
    std::vector<uint8_t> GenerateNonce();
    bool ValidateBlock(const std::string& hash);
    static std::string ComputeSHA256(const std::string& data);
    std::vector<std::string> ReadQuotesFromFile(const std::string& filePath);
    std::string GenerateHash(block* prevBlock, const std::string& quote, std::vector<uint8_t>& nonce);
    void AddBlock(block*& head, block*&tail, const std::string& quote, std::vector<uint8_t>& nonce);
    void CreateBlock(const std::string& quote, const std::vector<uint8_t>& nonce, const std::string& hash);
private:
    block* GenesisBlock;
    std::string filePath;
    std::vector<uint8_t> init;
};

#endif