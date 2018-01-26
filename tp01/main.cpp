#define HUFFMAN_IMPLEMENTATION
#define LZW_IMPLEMENTATION
#include "huffman.hpp"
#include "lzw.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <boost/program_options.hpp>
// #include <boost/filesystem.hpp>

using namespace std;
namespace po = boost::program_options;
// namespace fs = boost::filesystem;

void text(string path);

int main(int argc, char *argv[]) {
	string path, type;

	// Declare the supported options
	po::options_description desc("Allowed Options");
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

	if (type.compare("text")) {
		text(path);
	}
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
	// check test.cpp in compression
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