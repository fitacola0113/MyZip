#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "HUFF.h"
#include <queue>

std::map<char, int> reader(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return {};
    }
    std::map<char, int> CharFrequence;
    char c;
    while(file.get(c)) {
        CharFrequence[c]++;
    }
    file.close();
    return CharFrequence;
}

void genCods(Node* curNode, std::string codigoAcumulado, std::map<char, std::string>& Map) {
  
    if (curNode == nullptr) {
        return;
    }
    if (curNode->left == nullptr && curNode->right == nullptr) {
        Map[curNode->data] = codigoAcumulado;
        return; 
    }
    genCods(curNode->left, codigoAcumulado + "0", Map);
    genCods(curNode->right, codigoAcumulado + "1", Map);
}

std::map<char,std::string> HUFFcodes(std::map<char,int> CharFreq){
    std::map<char,std::string>codes;
    Node* root=BuildTree(CharFreq);
    for(auto& pair:CharFreq){
        genCods(root, "", codes);
    }
    return codes;
}

void compress(const std::string& inputFile, const std::string& outputFile,std::map<char, int> CharFreq,std::map<char, std::string> codes) {
    std::ifstream input(inputFile, std::ios::binary);
    std::ofstream output(outputFile, std::ios::binary);

    if (!input.is_open() || !output.is_open()) {
        std::cerr << "Error opening files." << std::endl;
        return;
    }

    size_t mapSize = CharFreq.size();
    output.write(reinterpret_cast<const char*>(&mapSize), sizeof(size_t));

    // Escreve os pares de (Caractere + Frequência)
    for (const auto& pair : CharFreq) {
        char character = pair.first;
        int frequency = pair.second;
        
        output.write(&character, sizeof(char));
        output.write(reinterpret_cast<const char*>(&frequency), sizeof(int));
    }

    unsigned char buffer = 0;
    int bitCount = 0;
    char readbyte;
    while (input.get(readbyte)) {
        std::string code = codes[readbyte];
        for (char bit : code) {
            buffer <<= 1;
            if (bit == '1') {
                buffer |= 1;
            }
            bitCount++;
            if (bitCount == 8) {
                output.put(buffer);
                buffer = 0;
                bitCount = 0;
            }
        }
    }
    if (bitCount > 0) {
        buffer <<= (8 - bitCount);
        output.put(buffer);
    }
    input.close();
    output.close();
}
void decompress(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream input(inputFile, std::ios::binary);
    std::ofstream output(outputFile, std::ios::binary);
    
    if (!input.is_open() || !output.is_open()) {
        std::cerr << "Error opening files." << std::endl;
        return;
    }
    std::map<char, int> CharFreq;
    int totalOriginalCharacters = 0; 

    size_t mapSize;
    input.read(reinterpret_cast<char*>(&mapSize), sizeof(size_t));

    for (size_t i = 0; i < mapSize; i++) {
        char character;
        int frequency;
        input.read(&character, sizeof(char));
        input.read(reinterpret_cast<char*>(&frequency), sizeof(int));
        CharFreq[character] = frequency;
        totalOriginalCharacters += frequency;
    }
    Node* root = BuildTree(CharFreq);
    Node* currentNode = root;
    int ExtracetedCharacters = 0;
    char readByte;
    while(input.get(readByte)&& ExtracetedCharacters < totalOriginalCharacters) {
        for (int i = 7; i >= 0; --i) {
            int bit = (readByte >> i) & 1;
            if (bit == 0) {
                currentNode = currentNode->left;
            } else {
                currentNode = currentNode->right;
            }
            if (currentNode->left == nullptr && currentNode->right == nullptr) {
                output.put(currentNode->data);
                currentNode = root;
                ExtracetedCharacters++;
                if (ExtracetedCharacters >= totalOriginalCharacters) {
                    break;
                }
            }
        }
    }
    input.close();
    output.close(); 
}

int compOrDecomp(const std::string& filename) {
    size_t pos = filename.find_last_of('.');
    if (pos != std::string::npos && pos != 0) {
        std::string extension = filename.substr(pos + 1);
        if (extension == "zip") {
            return 1;
        } else {
            return 0; 
        }
    }
    return -1; 
}
int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Erro: Ficheiro nao especificado." << std::endl;
        std::cerr << "Uso correto: " << argv[0] << " <nome_do_ficheiro>" << std::endl;
        return 1; // Termina o programa com código de erro
    }
    if (compOrDecomp(argv[1]) == 0) {
            std::string OriginalFile = argv[1];
        std::string OrFileNoPoint = OriginalFile;
        size_t pos = OrFileNoPoint.find_last_of('.');
        if(pos != std::string::npos && pos != 0) {
            OrFileNoPoint = OrFileNoPoint.substr(0, pos);
        
        }
    std::string CompressedFile = OrFileNoPoint + ".zip";
        std::map<char, int> CharFreq = reader(OriginalFile);
        std::map<char, std::string> codes = HUFFcodes(CharFreq);
        compress(OriginalFile, CompressedFile, CharFreq, codes);
        return 0;
    }
    else{
        std::string OriginalFile = argv[1];
        std::string OrFileNoPoint = OriginalFile;
        size_t pos = OrFileNoPoint.find_last_of('.');
        if(pos != std::string::npos && pos != 0) {
            OrFileNoPoint = OrFileNoPoint.substr(0, pos);

        }
        std::string DecompressedFile = OrFileNoPoint + ".txt";
        decompress(OriginalFile, DecompressedFile);
        return 0;

    }
    return 1;
}

