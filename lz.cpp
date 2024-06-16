#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>

std::string comprimir(const std::string& uncompressedData) {
    std::string compressedData = "";
    unsigned long long index = 0;
    const unsigned long long size = uncompressedData.size();
    unsigned long long par = -1;
    unsigned long long cantidad = 0;
    unsigned long long auxPar = 0;
    unsigned long long auxCantidad = 0;
    bool parSelected = false;

    while(index < size) {
        par = -1;
        cantidad = 0;
        auxPar = 0;
        auxCantidad = 0;
        parSelected = false;
        for(size_t j = 0; j < index; j++){
            auxCantidad = 0;
            parSelected = false;
            for(size_t i = auxPar; i < index; i++) {
                if(!parSelected && uncompressedData[i] == uncompressedData[index] && i != auxPar) {
                    auxPar = i;
                    parSelected = true;
                } 
                if(parSelected) {
                    if(uncompressedData[i] == uncompressedData[index + i - auxPar] && index + i - auxPar < size) {
                        auxCantidad++;
                    } else {
                        break;
                    }
                }
            }
            if(auxCantidad > cantidad) {
                par = auxPar++;
                cantidad = auxCantidad;
            }
        }

        if(par == -1) {
            compressedData += std::string(1, uncompressedData[index]) + ";";
            index++;
        } else {
            compressedData += std::to_string(par) + "," + std::to_string(cantidad) + ";";
            index += cantidad;
        }
        std::cout << "\rComprimiendo " << index  * 100 / size << " %";
        std::cout.flush();
    }

    std::cout << std::endl;

    return compressedData;
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
    
std::string estadisticasDeLaComprecion(const std::string& compressedText, const std::string& originalText) {
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
    if (!file.is_open()) {
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

    cstr = comprimir(str);

    auto inicio = std::chrono::high_resolution_clock::now();
    std::cout << estadisticasDeLaComprecion(cstr,str) << std::endl;
    auto fin = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duracion = fin - inicio;
    double duracion_minutos = duracion.count() / 60.0;
    std::cout << "Tardo " << duracion_minutos << " minutos." << std::endl;
    
    
    return 0;
}