#ifndef COD_HUFFMAN_H
#define COD_HUFFMAN_H
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <queue>
#include <unordered_map>
#include <bitset>

// Nodo del arbol
struct Node
{
	char simbolo;
	int frecuencia;
	Node *izq, *der;
};

// Funcion para asignar un nuevo nodo en el arbol
Node *getNode(char simbolo, int frecuencia, Node *izq, Node *der)
{
	Node *nodo = new Node();

	nodo->simbolo = simbolo;
	nodo->frecuencia = frecuencia;
	nodo->izq = izq;
	nodo->der = der;

	return nodo;
}

// Objeto de comparacion que será usado para ordenar la pila
struct comp
{
	bool operator()(Node *i, Node *d)
	{
		return i->frecuencia > d->frecuencia;
	}
};

class ArbolCompress
{
private:
	// Variable que almacena el texto codificado
	std::string str = "";
	// Variable que almacena el texto decodificado
	std::string textDec = "";
	//Variable que almacena el texto codificado en forma de bits
	std::string textCodBits = "";
	// Variable que contendrá un puntero a la raíz del arbol
	Node *raiz;

	// Revisa el arbol de Huffman y guarda los códigos en un mapa
	void encode(Node *raiz, std::string str, std::unordered_map<char, std::string> &CodigoHuffman)
	{
		if (raiz == nullptr)
		{
			return;
		}
		if (!raiz->izq && !raiz->der)
		{
			CodigoHuffman[raiz->simbolo] = str;
		}
		encode(raiz->izq, str + "0", CodigoHuffman);
		encode(raiz->der, str + "1", CodigoHuffman);
	}

	// Revisa el arbol y decodifica los simbolos codificados
	void decode(Node *raiz, const std::string &str)
	{
		Node *actual = raiz;
		int index = 0;

		while (index < str.size())
		{
			char bit = str[index];

			if (bit == '0')
			{
				actual = actual->izq;
			}
			else if (bit == '1')
			{
				actual = actual->der;
			}

			index++;

			if (!actual->izq && !actual->der)
			{
				textDec += actual->simbolo;
				actual = raiz; // Reiniciar desde la raíz para decodificar el próximo código
			}
		}
	}

	// Método para almacenar el valor Binario en un string de manera reducida.
	std::string toBit(std::string textToBit){
	for (int i = 0; i < textToBit.size(); i += 8) 
	{
        std::string subStr = textToBit.substr(i, 8);

        // Si la sub cadena no tiene 8 bits llenarla con ceros a la izquierda.
        if (subStr.size() < 8) {
            subStr = std::string(8 - subStr.size(), '0') + subStr;
        }

        std::bitset<8> bits(subStr);
        textCodBits += static_cast<char>(bits.to_ulong());
    }

    	return textCodBits;
	}

	// Método para eliminar los nodos del arbol para el método destructor de la clase
	void liberarArbol(Node *node)
	{
		if (node)
		{
			liberarArbol(node->izq);
			liberarArbol(node->der);
			delete node;
		}
	}

public:
	// Crea el arbol de Huffman y comdifica el texto ingresado
	ArbolCompress(std::string filePath, std::string encodeFileName)
	{
		// Extraemos la información del archivo como un string.
        std::ifstream file(filePath);
        if (!file.is_open())
        {
            std::cerr << "No se pudo abrir el archivo: " << filePath << std::endl;
            return;
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string data = buffer.str();

        // Creamos el archivo en el que guardaremos la información comprimida.
        std::ofstream encodeFile(encodeFileName);
        if (!encodeFile.is_open())
        {
            std::cerr << "No se pudo abrir el archivo para escribir: " << encodeFileName << std::endl;
            return;
        }


		// Cuenta la cantidad de veces que aparece cada simbolo y lo guarda en la tabla hash
		std::unordered_map<char, int> frecuencia;
		for (char simbolo : data)
		{
			frecuencia[simbolo]++;
		}

		// Crea una cola de prioridad para guardar los nodos del arbol
		std::priority_queue<Node *, std::vector<Node *>, comp> pq;

		// Agrega a la cola de prioridad los nodos de cada simbolo
		for (auto pair : frecuencia)
		{
			pq.push(getNode(pair.first, pair.second, nullptr, nullptr));
		}

		// Hacer todo el proceso hasta que gata mas de 1 nodo en la cola
		while (pq.size() != 1)
		{
			Node *izq = pq.top();
			pq.pop();
			Node *der = pq.top();
			pq.pop();
			int sum = izq->frecuencia + der->frecuencia;
			pq.push(getNode('\0', sum, izq, der));
		}
		raiz = pq.top();

		std::unordered_map<char, std::string> CodigoHuffman;
		encode(raiz, "", CodigoHuffman);

		// Generar el texto codificado
		for (char simbolo : data)
		{
			str += CodigoHuffman[simbolo];
		}

		toBit(str);

		encodeFile << textCodBits;
        file.close();
        encodeFile.close();
	}

	// Destructor de la clase ArbolCompress
	~ArbolCompress()
	{
		liberarArbol(raiz);
	}

    // Método público para decodificar la información dentro del arbol de Huffman
	void decodeTree()
	{
		int index = -1;
		decode(raiz, str);
	}
	
	// Retorna el texto codificado
	std::string getCode()
	{
		return str;
	}

	// Método que retorna el texto codificado y representando sus bits en char
	std::string getCodeBit()
	{
		return textCodBits;
	}

	// Método que retorna el texto decodificado
	std::string getDeCode()
	{
		return textDec;
	}

	// Método que retorna el tamaño del texto codificado en bits
	int getSizeCode()
	{
		return str.size();
	}

	// Retorna el tamaño del texto codificado y con representación de sus bits en char
	int getSizeCodeBit()
	{
		return textCodBits.size();
	}

	// Retorna el tamaño del texto decodificado en bits
	int getSizeDeCode()
	{
		return textDec.size();
	}
};

#endif