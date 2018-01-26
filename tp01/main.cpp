#define HUFFMAN_IMPLEMENTATION
#define LZW_IMPLEMENTATION
#include "huffman.hpp"
#include "lzw.hpp"
#include <boost/program_options.hpp>
#include <ctime>
#include <iostream>
#include <fstream>
#include <string>
// #include <boost/filesystem.hpp>

using namespace std;
namespace po = boost::program_options;
// namespace fs = boost::filesystem;

void text(string path);
int filesize(const string path);
double huffmanCompress(uint8_t * buffer, int length);
double lzwCompress(uint8_t * data, int length);

int main(int argc, char *argv[]) {
	string path, type;

	// Declare the supported options
	po::options_description desc{"Allowed Options"};
	desc.add_options()
		("help,h", "produce help message")
		("path,p", po::value<string>(&path), "Path to the file to compress")
		("type,t", po::value<string>(&type), "The type of file to compress")
	;
	po::variables_map vm;
	try {
		po::store(po::parse_command_line(argc, argv, desc), vm);
	}
	catch (boost::exception &) {
		cout << "Invalid Option!!" << endl;
		cout << desc << endl;
		return -1;
	}
	po::notify(vm);

	if (vm.count("help")) {
		cout << desc << endl;
		return 1;
	}

	if (vm.count("type") && vm.count("path")) {
		if (type == "text") {
			cout << "hello" << endl;
			text(path);
		}
	}
	else {
		cout << "You must specify type and path" << endl;
		cout << desc << endl;
		return -2;
	}
	return 0;
}

int filesize(const string path) {
	ifstream in(path, ifstream::ate | ifstream::binary);

	return in.tellg();
}

void text(string path) {
	ifstream inFile;

	inFile.open(path, ifstream::binary);
	if (!inFile) {
		cout << "unable to open" << path << endl;
		exit(1);
	}
	int length = filesize(path);

	char *buffer = new char [length];

	inFile.read(buffer, length);

	if (inFile) {
		cout << "all characters cast succesfully" << endl;
	} else  {
		cout << "error: only " << inFile.gcount() << " could be read" << endl;
	}

	// Huffman Compression
	clock_t start_huff = clock();
	double taux_huff = huffmanCompress((uint8_t*) buffer, length);
	clock_t end_huff = clock();
	double time_huff = double(end_huff - start_huff) / CLOCKS_PER_SEC;

	// LZW compression
	clock_t start_lzw = clock();
	double taux_lzw = lzwCompress((uint8_t*) buffer, length);
	clock_t end_lzw = clock();
	double time_lzw = double(end_lzw - start_lzw) / CLOCKS_PER_SEC;

	cout << "Temps de compression Huffman " << time_huff << endl;
	cout << "Taux de compression Huffman " << taux_huff << endl;

	cout << "Temps de compression Huffman " << time_lzw << endl;
	cout << "Taux de compression Huffman " << taux_lzw << endl;

	delete[] buffer;
	inFile.close();
	cout << path << endl;
}

double huffmanCompress(uint8_t * data, int length) {
	int compressedSizeBytes = 0;
    int compressedSizeBits  = 0;
    std::uint8_t * compressedData = nullptr;
    std::vector<std::uint8_t> uncompressedBuffer(length, 0);

    // Compress:
    huffman::easyEncode(data, length, &compressedData,
                        &compressedSizeBytes, &compressedSizeBits);

	// Check if there were any errors
	if (compressedData) {
		cout << "Huffman compressed successfully" << endl;
	} else {
		cout << "ERROR: not compressed successfully" << endl;
	}

	// Return compression ratio
	return 1 - compressedSizeBytes/length;
}

double lzwCompress(uint8_t * data, int length) {
	int compressedSizeBytes = 0;
    int compressedSizeBits  = 0;
    std::uint8_t * compressedData = nullptr;
    std::vector<std::uint8_t> uncompressedBuffer(length, 0);

    // Compress:
    lzw::easyEncode(data, length, &compressedData,
                        &compressedSizeBytes, &compressedSizeBits);

	// Check if there were any errors
	if (compressedData) {
		cout << "Huffman compressed successfully" << endl;
	} else {
		cout << "ERROR: not compressed successfully" << endl;
	}

	// Return compression ratio
	return 1 - compressedSizeBytes/length;
}
