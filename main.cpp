#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include "CodHuffman.h"

int main(int argc, char const *argv[])
{
	std::ifstream file("dna.5MB.txt");
	if (!file.is_open())
	{
		std::cerr << "Error al abrir el archivo " << std::endl;
		return 1;
	}

	std::string text;
	std::string line;
	while (std::getline(file, line))
	{
		text += line;
	}

	file.close();

	// Medir el tiempo de compresión
	auto start = std::chrono::high_resolution_clock::now(); // Tiempo de inicio

	int tam = crearArbol(text);

	auto end = std::chrono::high_resolution_clock::now(); // Tiempo de finalización

	// Calcular la duración
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << "Huffman" << ";" << tam << ";" << duration.count() << std::endl;

	return 0;
}
