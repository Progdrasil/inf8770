#define HUFFMAN_IMPLEMENTATION
#define LZW_IMPLEMENTATION
#include "huffman.hpp"
#include "lzw.hpp"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void text(string path);

int main(int argc, char *argv[]) {
	if (argc != 3) {
		cout << "Il faut inclure le path comme premier argument et le type comme deuxieme" << endl;
		return 1;
	}

	string type = argv[2];
	string path = argv[1];

	if (type == "text") {
		text(path);
	}

	cout << argv[0] << endl;
	return 0;
}

void text(string path) {
	ifstream inFile;

	inFile.open(path);
	if (!inFile) {
		cout << "unable to open" << path << endl;
		exit(1);
	}
	inFile.seekg (0, inFile.end);
    int length = inFile.tellg();
    inFile.seekg (0, inFile.beg);

	char *buffer = new char [length];

	inFile.read(buffer, length);

	if (inFile) {
		cout << "all characters cast succesfully" << endl;
	} else  {
		cout << "error: only " << inFile.gcount() << " could be read" << endl;
	}

	// TODO: Compress file here

	uint8_t ** cBuffer;
	int *compressedSizeBytes;
	int *compressedSizeBits;
	huffman::easyEncode((uint8_t*) buffer, length, cBuffer, compressedSizeBytes, compressedSizeBits);

	if (cBuffer) {
		cout << "compressed successfully" << endl;
	} else {
		cout << "ERROR: not compressed successfully" << endl;
	}
	// TODO: calculate time of compression
	// TODO: calculate compression ratio


	delete[] buffer;
	inFile.close();
	cout << path << endl;
}
