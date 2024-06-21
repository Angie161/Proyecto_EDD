#ifndef LZ24_H
#define LZ24_H

#include <iostream>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <chrono>
#include <string>

class LZ24 {
    public:
        LZ24() : searchWindowSize(750), cicoSize(1000000) {}                                                            // Tamaño por defecto de la ventana de busqueda y ciclo del trie.

        LZ24(size_t searchWindowSize, size_t cicoSize) : searchWindowSize(searchWindowSize), cicoSize(cicoSize) {}      // Constuctor personalizado para ventanas de mayor tamaño.

        ~LZ24(){}
        
        void compress(std::string filePath, std::string compressFileName) {                                             // Parametros : (Nombre del archivo a comprimir, nombre del archivo comprimido).
            // Extraemos la información del archivo como un string.
            std::ifstream file(filePath);
            if(!file.is_open()) {
                std::cerr << "No se pudo abrir el archivo: " << filePath << std::endl;
                return;                                                                         
            }
            std::stringstream buffer;
            buffer << file.rdbuf();
            file.close();
            std::string data = buffer.str();

            // Hacemos un preprocesado para eliminar los caracteres especiales que usaremos.
            size_t pos;
            while((pos = data.find(";")) != std::string::npos) {
                data.replace(pos,1,"#p#");
            }
            while((pos = data.find(",")) != std::string::npos) {
                data.replace(pos,1,"#c#");
            }

            // Creamos el archivo en el que guardaremos la información comprimida.
            std::ofstream compressFile(compressFileName);
            if (!compressFile.is_open()) {
                std::cerr << "No se pudo abrir el archivo para escribir: " << compressFileName << std::endl;
                return;
            }

            // Declaramos las variables a usar.
            Trie trie;
            size_t longestSavedWord = 1;
            std::string word;
            std::pair<ssize_t, ssize_t> par;
            std::string parStr;
            size_t contador = 0;
            int percentage;
            bool findWord = true;
            bool writingPhrase = false;

            // Iniciamos el proceso de compresión.
            for(int index = 0; index < data.size(); index++) {
                std::cout << "\rCompressing   " << (index  * 100 / data.size()) << " %";                                // Mostramos el porcentaje de la comprecion en pantalla.

                for(int i = 0; i < longestSavedWord; i++) {
                    word = data.substr(index,longestSavedWord - i);
                    par = trie.search(word);
                    parStr = numberToString(index - par.first) + "," + numberToString(par.second) + ";";
                    if(par.second != 0 && word.size() > parStr.size()) {                                                // Mi palabra esta en mi trie y sale acuenta cambiarlo por un par?
                        if(writingPhrase) compressFile << ";" + parStr;
                        else compressFile << parStr;
                        writingPhrase = false;
                        index += word.size() - 1;
                        break;
                    } else if(i == longestSavedWord - 1) {                                                              // No? entonces nomas escribe la palabra mas pequeña que no coincidia.
                        compressFile << word;
                        writingPhrase = true;
                    }
                }
                contador = 0;
                findWord = true;
                while(findWord) {                                                                                       // Mientras encontremos coincidencias.
                    contador++;
                    findWord = false;
                    for(int i = index + 1; i < index + std::min(searchWindowSize, data.size()) && index % cicoSize < searchWindowSize; i++) { 
                        if(data.substr(index, contador) == data.substr(i, contador)) {
                            if(contador > longestSavedWord) longestSavedWord = contador;                                // Si encontre una coincidencia más larga de las que tengo, la cambio.
                            findWord = true;
                        }
                    }
                }
                trie.insert(data.substr(index, longestSavedWord), index);                                               // Insertamos la coincidencia en el trie.
                if(index % cicoSize == 0) {
                    trie.clear();                                                                                       // Limpieamos el trie cda ciclo.
                }
            }
            std::cout << "\rCompressing   100 % " << std::endl;
            compressFile.close();
        }

        void decompress(std::string filePath, std::string fileName) {                                                   // Parametros : (Nombre del archivo a descomprimir, nombre del archivo descomprimido).
            // Extraemos la información del archivo como un string.
            std::ifstream compressFile(filePath);
            if(!compressFile.is_open()) {
                std::cerr << "No se pudo abrir el archivo: " << filePath << std::endl;
                return;                                                                         
            }
            std::stringstream buffer;
            buffer << compressFile.rdbuf();
            compressFile.close();
            std::string compressedData = buffer.str();

            // Creamos el archivo en el que guardaremos la información comprimida.
            std::ofstream file(fileName);
            if (!file.is_open()) {
                std::cerr << "No se pudo abrir el archivo para escribir: " << fileName << std::endl;
                return;
            }

            // Inicializamos nuestras variables.
            ssize_t aux;
            ssize_t vA;
            ssize_t vB;
            ssize_t vC;
            ssize_t size;
            std::string auxFile = "";

            std::cout << "Descomprimiendo   0 %";
            for(ssize_t i = 0; i < compressedData.size();) {                                                              // Paso por todos los caracteres de mi archivo comprimido.
                std::cout << "\rDescomprimiendo   " << (i  * 100 / compressedData.size()) << " %";
                aux = i;
                vA = i;
                vB = -1;
                vC = -1;
                size = -1;
                while(true) {                                                                                              // Asigno mis variables para identificar si es una palabra o un par.
                    if(compressedData[aux] == ';') {
                        vC = aux;
                        size = aux - i;
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
                if(vB == -1) {                                                                                              // Si es una palabra, la agrego.
                    auxFile += compressedData.substr(vA, size);
                } else {                                                                                                    // Si no lo es, realizo las acciones para copiar un par.
                    std::string p = "";
                    std::string c = "";
                    for(size_t k = vA; k < vB; k++) {
                        p += compressedData[k];
                    } 
                    for(size_t k = vB + 1; k < vC; k++) {
                        c += compressedData[k];
                    } 
                    ssize_t co = auxFile.size();
                    for(size_t k = 0; k < stringToNumber(c); k++) {
                        auxFile += auxFile[co - stringToNumber(p) + k];
                    }
                }
                i += size + 1;
            }

            std::cout << "\rDescomprimiendo   100 %" << std::endl;

            // Hacemos un postprocesado para recuperar los caracteres especiales que habian.
            size_t pos;
            while((pos = auxFile.find("#p#")) != std::string::npos) {
                auxFile.replace(pos,3,";");
            }
            while((pos = auxFile.find("#c#")) != std::string::npos) {
                auxFile.replace(pos,3,",");
            }

            file << auxFile;
            file.close();
        }

        void doTest(std::string fileName, std::string fileToCompress, int cantidad) {                                         // Parametros(Nombre del archivo que tendra los resultados, el archivo a comprimir, la cantidad de veces.)
            std::ofstream file(fileName);
            if (!file.is_open()) {
                std::cerr << "No se pudo abrir el archivo para escribir: " << fileName << std::endl;
                return;
            }
            file << "tecnica;tamaño;tiempo_ejecucion\n";

            for(int i = 0; i < cantidad; i++) {
                auto start = std::chrono::high_resolution_clock::now(); 
                compress(fileToCompress, "compressFile.lz24");
                auto end = std::chrono::high_resolution_clock::now(); 

                std::ifstream compressFile("compressFile.lz24");
                if (!compressFile.is_open()) {
                    std::cerr << "No se pudo abrir el archivo: compressFile.lz24" << std::endl;
                    return;
                }
                std::stringstream buffer;
                buffer << compressFile.rdbuf();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
                file << "Lempel-Ziv" << ";" << buffer.str().size() << ";" << duration.count() << std::endl;
                compressFile.close();
            }
            file.close();
        }

    private:
        size_t searchWindowSize;
        size_t cicoSize;

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
                
                std::pair<ssize_t, ssize_t> search(const std::string& word) {
                    TrieNode* node = root;
                    for (char c : word) {
                        if (node->children.find(c) == node->children.end()) {
                            return std::make_pair(-1LL,0);
                        }
                        node = node->children[c];
                    }
                    return std::make_pair(node->donde, node->altura);
                }
                
                void clear() {
                    delete root;
                    root = new TrieNode();
                }

            private:
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
                
                TrieNode* root;
        };

        std::string numberToString(size_t number) {         // Para convertir un numero en un string, ejemplo: 97 -> "a" , 685 -> "aa".
            std::string str = "";
            while(number != 0) {
                str += number | 0x00;                       // Agrego al string un char igual al ultimo byte de mi numero.
                number >>= 8;                               // Muevo los bits de mi número 8 espacios a la derecha.
            }
            return str;
        }

        size_t stringToNumber(std::string str) {            // Para desconvertir un numero en un string, ejemplo: "a" -> 97 , "aa" -> 685.
            size_t number = 0;
            for (int i = str.size(); 0 <= i; i--) {
                number <<= 8;                               // Muevo los bits de mi número 8 espacios a la izquieda.
                number |= (unsigned char) str[i];           // Agrego los bits del ultimo char del string a mi número con el operador OR.
            }
            return number;
        }

};

#endif
