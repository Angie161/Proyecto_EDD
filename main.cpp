#include "CodHuffman.h"
#include <iostream>

int main(int argc, char const *argv[])
{
	std::cout << "\n\n\n--------------------------------------------------";
	std::cout << "\n      Compresion con el metodo Huffman";
	std::cout << "\n\n\n--------------------------------------------------";
	std::string text;
	std::cout << "\nIngrese el mensaje que desea comprimir: ";
	std::getline(std::cin, text);
	crearArbol(text);
	std::cout << "\n";
	return 0;
}