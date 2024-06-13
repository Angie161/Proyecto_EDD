#ifndef COD_HUFFMAN_H
#define COD_HUFFMAN_H
#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>

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

// Recisa el arbol de Huffman y guarda los códigos en un mapa
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
void decode(Node *raiz, int &index, std::string str)
{
	if (raiz == nullptr)
	{
		return;
	}
	if (!raiz->izq && !raiz->der)
	{
		std::cout << raiz->simbolo;
		return;
	}
	index++;
	if (str[index] == '0')
	{
		decode(raiz->izq, index, str);
	}
	else
	{
		decode(raiz->der, index, str);
	}
}

// Crea el arbol de Huffman y retorna el tamaño del texto comprimido en bits
int crearArbol(const std::string &text)
{
    // Cuenta la cantidad de veces que aparece cada simbolo y lo guarda en la tabla hash
    std::unordered_map<char, int> frecuencia;
    for (char simbolo : text)
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
    Node *raiz = pq.top();

    std::unordered_map<char, std::string> CodigoHuffman;
    encode(raiz, "", CodigoHuffman);

    // Generar el texto codificado
    std::string str = "";
    for (char simbolo : text)
    {
        str += CodigoHuffman[simbolo];
    }

    // Retorna el tamaño del texto codificado en bits
    return str.size();
}

#endif