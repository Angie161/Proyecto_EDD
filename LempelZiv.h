#include <iostream>
#include <unordered_map>
#include <memory>
#include <fstream>
#include <sstream>
#include <chrono>
#include <vector>
#include <string>

class TrieNode {
public:
    std::unordered_map<char, TrieNode*> children;
    ssize_t donde; 
    ssize_t altura;

    TrieNode() : altura(0), donde(-1LL) {}
    TrieNode(ssize_t altura, ssize_t donde) : altura(altura), donde(donde) {}
    ~TrieNode() {
        for (auto& pair : children) {
            delete pair.second;
        }
    }
};

class Trie {
public:
    Trie() {
        root = new TrieNode();
    }

    void insert(const std::string& word, ssize_t indice) {
        TrieNode* node = root;
        for (char c : word) {
            if (node->children.find(c) == node->children.end()) {
                node->children[c] = new TrieNode(node->altura + 1, indice);
            }
            node->donde = indice;
            node = node->children[c];
        }
    }
    
    std::pair<ssize_t, ssize_t> search(const std::string& word, ssize_t indice) {
        TrieNode* node = root;
        for (char c : word) {
            if (node->children.find(c) == node->children.end()) {
                if(c != ',' && c != ';') {                                                                      //////// Forma tonta de lidear con las , ////////
                    node->children[c] = new TrieNode(node->altura + 1, indice);
                }
                return std::make_pair(-1LL,0);
            }
            node->donde = indice;
            node = node->children[c];
        }
        return std::make_pair(node->donde, node->altura);
    }
    
    void clear() {
        delete root;
        root = new TrieNode();
    }

private:
    TrieNode* root;
};

std::string comprimir(const std::string& data, bool conCiclo) {
    std::cout << "Comprimiendo   0 %";

    std::string response = "";
    ssize_t index = 0;
    ssize_t size = data.size();
    ssize_t largoDelCiclo = !conCiclo ? size : 1000000;
    bool frase = false;
    Trie trie;
    std::pair<ssize_t, ssize_t> last = std::make_pair(-1LL, 0);
    std::pair<ssize_t, ssize_t> par = std::make_pair(-1LL, 0);
    const std::pair<ssize_t, ssize_t> noEstaba = std::make_pair(-1LL, 0);
    
    for(int w = 0; w < size; w += largoDelCiclo) {
        while(index < std::min(size, w + largoDelCiclo)) {
            last = std::make_pair(-1LL, 0);
            par = std::make_pair(-1LL, 0);
            for(ssize_t largo = 1; true; largo++) {
                par = trie.search(data.substr(index, largo), index);
                if(par == noEstaba || index + largo == std::min(size, w + largoDelCiclo)) {
                    if(last == noEstaba) {
                        if(data.substr(index, 1) != "," && data.substr(index, 10) != ";") {       
                            trie.insert(data.substr(index, largo + 1), index);
                            response += std::string(1,data[index++]);
                            frase = true;
                        } else if(data.substr(index, 1) == ";") {                                     /////////////////////////////////////////////////
                            response += ";;";                      //////// Forma tonta de lidear con las ; ////////
                            index++;                   
                            frase = false;             /////////////////////////////////////////////////
                        } else {        
                            if(frase) {
                            response += ";,;";   
                            } else {
                            response += ",;";   
                            }                           
                            frase = false;     
                            index++;                            
                        }

                    } else if(last.second > (std::to_string(index - last.first) + "," + std::to_string(last.second) + ";").size()) {
                        if(frase) {
                            response += ";" + std::to_string(index - last.first) + "," + std::to_string(last.second) + ";"; 
                        } else {
                            response += std::to_string(index - last.first) + "," + std::to_string(last.second) + ";"; 
                        }
                        for(int k = 0; k < last.second; k++) {
                            trie.insert(data.substr(index, (largo + last.second) + k + 10), index);
                            frase = false;
                            index ++;
                        }
                    } else {
                        trie.insert(data.substr(index, largo + 10), index);
                        response += std::string(1,data[index++]);
                        frase = true;
                    }
                    break;
                }
                last = par;
            }
            if(!conCiclo) {
                std::cout << "\rComprimiendo   " << (index * 100 / size) << " %";
            }
        }
        if(conCiclo) {
            trie.clear();
            std::cout << "\rComprimiendo   " << (index  * 100 / size) << " %";
        }
    }

    std::cout << "\rComprimiendo   100 %" << std::endl;

    return response;
}

std::string descomprimir(const std::string& compressedData) {
    std::string uncompressedData = "";
    std::cout << "Descomprimiendo   0 %";

    for(ssize_t i = 0; i < compressedData.size();) {
        ssize_t aux = i;
        ssize_t vA = i;
        ssize_t vB = -1;
        ssize_t vC = -1;
        ssize_t size = -1;
        while(true) {
            if(compressedData.substr(aux,2) == ",;") {        
                vB = -2;                                                
                vC = aux + 2;                                           
                size = 2;
                break;
            }if(compressedData.substr(aux,2) == ";;" && aux == i) {     /////////////////////////////////////////////////
                vB = -3;                                                //////// Forma tonta de lidear con las , ////////
                vC = aux + 2;                                           /////////////////////////////////////////////////
                size = 2;
                break;
            }
            if(compressedData[aux] == ';' && aux != vA) {
                vC = aux;
                size = aux - i + 1;
                break;
            }
            if(compressedData[aux] == ',') {
                vB = aux;
            }
            if(aux == compressedData.size()) {
                vC = aux - 1;
                size = aux - i + 1;
                break;  
            }
            aux++;
        }
        if(vB == -1) {
            std::string a = "";
            for(size_t k = vA; k < size - 1 + vA; k++) {
                a += compressedData[k];
            } 
            uncompressedData += a;
        } else if(vB == -2) {
            uncompressedData += ",";
        } else if(vB == -3) {
            uncompressedData += ";";
        } else {
            std::string p = "";
            std::string c = "";
            for(size_t k = vA; k < vB; k++) {
                p += compressedData[k];
            } 
            for(size_t k = vB + 1; k < vC; k++) {
                c += compressedData[k];
            } 
            for(size_t k = 0; k < std::stoi(c); k++) {
                uncompressedData += uncompressedData[i - std::stoi(p) + k - 1];
            }
        }
        i += size;
        std::cout << "\rDescomprimiendo   " << (i  * 100 / compressedData.size()) << " %";
    }

    std::cout << "\rDescomprimiendo   100 %" << std::endl;

    return uncompressedData;
}

std::string estadisticasDeLaCompresion(const std::string& compressedText, const std::string& originalText) {
    std::string response = "";

    int bytes = originalText.size() - compressedText.size();

    if(bytes >= 0) {
        response += "Ahora la informacion ocupa " + std::to_string(bytes) + " bytes menos de espacio. :)";
    } else {
        response += "Ahora la informacion ocupa " + std::to_string(-bytes) + " bytes mas de espacio. :(";
    }

    return response;
}


std::string readFileToString(const std::string& filePath) {
    // Crear un objeto ifstream para abrir el archivo
    std::ifstream file(filePath);

    // Verificar si el archivo se abrió correctamente
    if(!file.is_open()) {
        std::cerr << "No se pudo abrir el archivo: " << filePath << std::endl;
        return "";
    }

    // Utilizar un stringstream para almacenar el contenido del archivo
    std::stringstream buffer;
    buffer << file.rdbuf();

    // Cerrar el archivo
    file.close();

    // Devolver el contenido del buffer como un string
    return buffer.str();
}
