#include <vector>
#include <string>
#include <random>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>
#include "quotechain.h"
#include <openssl/sha.h>

// utility to print nonce
std::string GetNonce(const std::vector<uint8_t>& nonce) {
    std::stringstream ss;
    for (auto byte : nonce) {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    }
    return ss.str();
}

QuoteChain::QuoteChain(std::string fp) : filePath(fp), init(32) {
    std::random_device rd;
    std::mt19937 gen(rd());
    // using a byte array, each byte can hold a value [0, 255]
    std::uniform_int_distribution<uint8_t> dis(0, 255);

    std::stringstream ss;
    for (int bt = 0; bt < 32; ++bt) {
        uint8_t byte = dis(gen);
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
    }
    std::string initialHash = ss.str();

    this->GenesisBlock = new block();
    this->GenesisBlock->blockhash = initialHash;
}

void QuoteChain::PrintChain(block* head) {
    block* temp = head;
    int blockNumber = 0;
    
    while (temp->next != nullptr) {
        std::cout << "Block " << blockNumber << std::endl;
        std::cout << "Quote: " << temp->quote << std::endl;
        std::cout << "Nonce: " << GetNonce(temp->nonce) << std::endl;
        std::cout << "Hash: " << temp->blockhash << std::endl;
        std::cout << std::endl;

        // shift
        blockNumber++;
        temp = temp->next;
    }

    std::cout << "Quote: " << temp->quote << std::endl;
    std::cout << "Hash: " << temp->blockhash << std::endl;
}

std::vector<std::string> QuoteChain::ReadQuotesFromFile(const std::string& filePath) {
    std::vector<std::string> quotes;
    std::ifstream file(filePath);
    std::string line;

    if (file.is_open()) {
        while (getline(file, line)) {
            if(!line.empty()) {
                quotes.push_back(line);
            }
        }
        file.close();
    } else {
        std::cerr << "Unable to Open File: " << filePath;
    }

    return quotes;
}

std::string QuoteChain::GenerateQuote() {
    std::vector<std::string> quotes = ReadQuotesFromFile(this->filePath);

    std::random_device rd;
    std::default_random_engine engine(rd());
    std::uniform_int_distribution<size_t> dist(0, quotes.size() - 1);

    return quotes[dist(engine)];
}

std::vector<uint8_t> QuoteChain::GenerateNonce() {
    std::vector<uint8_t> nonce(256);

    // pseudorandomness 
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dis(0, 255);

    for (int bt = 0; bt < 256; ++bt) {
        nonce[bt] = dis(gen);
    }
    
    return nonce;
}

bool QuoteChain::ValidateBlock(const std::string& hash) {
    // get first 2 hex chars
    char first_byte = strtol(
        hash.substr(0, 2).c_str(), 
        nullptr, 16
    );
    
    return (first_byte & 0xF8) == 0;
}

std::string QuoteChain::ComputeSHA256(const std::string& data) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, data.c_str(), data.size());
    SHA256_Final(hash, &sha256);

    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    } 

   return ss.str();
}

std::string QuoteChain::GenerateHash(block* prevBlock, const std::string& quote, std::vector<uint8_t>& nonce) {
    std::string prevHash = (prevBlock != nullptr) ? prevBlock->blockhash : GenesisBlock->blockhash;
    
    std::string hash;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint8_t> dis(0, 255);

    do {

        for (auto& n : nonce) {
            n = dis(gen);
        }

        std::stringstream nonceStr;
        for (auto& n : nonce) {
            nonceStr << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(n);
        }

        std::string blockData = prevHash + nonceStr.str() + quote;
        hash = ComputeSHA256(blockData);
    } while(!ValidateBlock(hash));

    return hash;
}

void QuoteChain::AddBlock(block*& head, block*&tail, const std::string& quote, std::vector<uint8_t>& nonce) {
    std::string blockHash = GenerateHash(tail, quote, nonce);

    block* newBlock = new block();
    newBlock->quote = quote;
    newBlock->blockhash = blockHash;
    newBlock->nonce = nonce;

    if (tail != nullptr) {
        tail->next = newBlock;
    } else {
        head = newBlock;
    }
    tail = newBlock;
}