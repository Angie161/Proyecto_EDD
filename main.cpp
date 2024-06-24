#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include "CodHuffman.h"
//#include "LZ24.h"

int main(int argc, char const *argv[])
{

	// ---------------------- HUFFMAN -----------------------------------

	// Medir el tiempo de codificacion
	auto start_Huffman = std::chrono::high_resolution_clock::now(); // Tiempo de inicio
	ArbolCompress arbol_Huffman("dna.5MB.txt", "encoded.txt");
	auto end_Huffman = std::chrono::high_resolution_clock::now(); // Tiempo de finalización
	// Calcular la duración
	auto duration_Huffman = std::chrono::duration_cast<std::chrono::milliseconds>(end_Huffman - start_Huffman);

	std::ifstream encodeFile("encoded.txt", std::ios::binary | std::ios::ate);
	std::streamsize encodeSize = encodeFile.tellg();
	encodeFile.close();
	std::cout << "Huffman" << ";10MB" << ";codificacion" << ";" << encodeSize << ";" << duration_Huffman.count() << std::endl;

	// Medir el tiempo de decodificacion
	auto startDes_Huffman = std::chrono::high_resolution_clock::now(); // Tiempo de inicio
	arbol_Huffman.decodeTree();
	auto endDes_Huffman = std::chrono::high_resolution_clock::now(); // Tiempo de finalización
	// Calcular la duración
	auto durationDes_Huffman = std::chrono::duration_cast<std::chrono::milliseconds>(endDes_Huffman - startDes_Huffman);
	std::cout << "Huffman" << ";10MB" << ";decodificacion" << ";" << arbol_Huffman.getSizeDeCode() << ";" << durationDes_Huffman.count() << std::endl;

	// ---------------------- LEMPEL ZIV -----------------------------------
	// Medir el tiempo de compresión
	//auto start_LempelZiv = std::chrono::high_resolution_clock::now(); // Tiempo de inicio
	//LZ24 arbol_LempelZiv;
	//arbol_LempelZiv.compress("dna.5MB.txt", "compressed.lz24");
	//auto end_LempelZiv = std::chrono::high_resolution_clock::now(); // Tiempo de finalización
	// Calcular la duración
	//auto duration_LempelZiv = std::chrono::duration_cast<std::chrono::milliseconds>(end_LempelZiv - start_LempelZiv);

	// Obtener el tamaño del archivo comprimido
	//std::ifstream compressFile("compressed.lz24", std::ios::binary | std::ios::ate);
	//std::streamsize compressedSize = compressFile.tellg();
	//compressFile.close();

	//std::cout << "LempelZiv" << ";10MB" << ";compresion" << ";" << compressedSize << ";" << duration_LempelZiv.count() << std::endl;

	// Medir el tiempo de descompresión
	//auto startDes_LempelZiv = std::chrono::high_resolution_clock::now(); // Tiempo de inicio
	//arbol_LempelZiv.decompress("compressed.lz24", "decompressed.txt");
	//auto endDes_LempelZiv = std::chrono::high_resolution_clock::now(); // Tiempo de finalización
	// Calcular la duración
	//auto durationDes_LempelZiv = std::chrono::duration_cast<std::chrono::milliseconds>(endDes_LempelZiv - startDes_LempelZiv);

	// Obtener el tamaño del archivo descomprimido
	//std::ifstream decompressedFile("decompressed.txt", std::ios::binary | std::ios::ate);
	//std::streamsize decompressedSize = decompressedFile.tellg();
	//decompressedFile.close();

	//std::cout << "LempelZiv" << ";10MB" << ";descompresion" << ";" << decompressedSize << ";" << durationDes_LempelZiv.count() << std::endl;

	//file.close();
	return 0;
}
