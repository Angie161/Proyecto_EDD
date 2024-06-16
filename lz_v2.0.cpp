#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <vector>
#include <unordered_map>
#include <string>

std::string comprimir(const std::string& data) {
    std::cout << "Comprimiendo..." << std::endl;
    // Primera parte.
    std::unordered_map<char,std::unordered_map<long long, long long>> auxData; 
    std::vector<std::pair<char, std::unordered_map<long long, long long>>> semiData;

    const long long size = data.size();
    for(long long i = 0; i < size; i++) {
        semiData.push_back(std::make_pair(data[i], auxData[data[i]]));
        auxData[data[i]][i] = 1;
        if((i * 100 / size) % 2 == 0 || i == 1) {
            std::cout << "\rPrimera fase al " << (float) (i  * 100 / size) << " %";
            std::cout.flush();
        }
    }
    std::cout << std::endl;

    /*for(auto a : semiData) {
        std::cout << a.first;
        for(auto b : a.second) {        //////////////////////////////////////
            std::cout << " " << b.first;      ///////// Verificar semiData /////////
        }                               //////////////////////////////////////
        
        std::cout << std::endl;
    }*/

    // Segunda parte.
    std::string response = "";
    long long consecutividad = 0;
    long long maximaConsecutividad = 0;
    long long posicion = -1;
    long long maximaPosicion = -1;
    for(long long i = 0; i < semiData.size();) { // Recorror cada elemento de mi semiData
        if(semiData[i].second.empty()) {             // Si esta vacio, entonces imprimo su caracter y ya
            response += std::string(1, semiData[i].first) + ";";
            i++;
        } else {   // Si no esta vacio ciclamos 
            maximaConsecutividad = 0;
            maximaPosicion = -1;
            for(auto v : semiData[i].second) {
                posicion = v.first;
                consecutividad = 0;
                for(long long j = 0;true; j++) {
                    if(i + j < semiData.size()) {
                        if(semiData[i + j].second.end() != semiData[i + j].second.find(v.first + j)) {
                            consecutividad++;
                        } else {
                            break;
                        }
                    } else {
                        break;
                    }
                }
                if(consecutividad > maximaConsecutividad) {
                    maximaPosicion = posicion;
                    maximaConsecutividad = consecutividad;
                }
            }
            i += maximaConsecutividad;
            response += std::to_string(maximaPosicion) + "," + std::to_string(maximaConsecutividad) + ";";
        }
        if((i * 100 / size) % 2 == 0 || i == 1) {
            std::cout << "\rSegunda fase al " << (float) (i  * 100 / size) << " %";
            std::cout.flush();
        }
    }

    std::cout << std::endl;

    return response;
}

std::string descomprimir(const std::string& compressedData) {
    std::string uncompressedData = "";

    for(size_t i = 0; i < compressedData.size() - 1;) {
        int aux = i;
        int vA = i;
        int vB = -1;
        int vC;
        int size;
        while(true) {
            if(compressedData[aux] == ',') {
                vB = aux;
            }
            if(compressedData[aux] == ';' && aux != vA) {
                vC = aux;
                size = aux - i + 1;
                break;
            }
            aux++;
        }
        if(vB == -1) {
            uncompressedData += compressedData[aux - 1];
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
                uncompressedData += uncompressedData[std::stoi(p) + k];
            }
        }
        i += size;
    }

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


int main(int argc, char* argv[]) {
    std::string str;
    std::string cstr;

    if(argc < 2) {
        std::cout << "No se ingreso archivo a ejecutar, se ocupara la frase \"Pedro Pablo Perez Pereira\"" << std::endl;
        str = "Pedro Pablo Perez Pereira";
    } else {
        str = readFileToString(argv[1]);
    }
    // std::cout << "Primero una prueba de correcta comprecion y descomprecion de \"No seeee nada de de este estadio\"" << std::endl;
    // std::string c = comprimir("No seeee nada de de este estadio");
    // std::cout << c << std::endl;
    // std::cout << descomprimir(c) << std::endl;

    auto inicio = std::chrono::high_resolution_clock::now();
    cstr = comprimir(str);
    auto fin = std::chrono::high_resolution_clock::now();

    std::cout << estadisticasDeLaCompresion(cstr,str) << std::endl;

    std::chrono::duration<double> duracion = fin - inicio;
    double duracion_minutos = duracion.count() / 60.0;
    std::cout << "Tardo " << duracion_minutos << " minutos." << std::endl;
    
    
    return 0;
}