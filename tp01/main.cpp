/*******************************************************************************
	TP01: comparaison codage Huffman et LZW

	Auteurs:
		- Rene Leveille
		- Thibault Noilly

	Cree le: 12/01/2018
	Modifie le: 26/01/2018
*******************************************************************************/

#define HUFFMAN_IMPLEMENTATION
#define LZW_IMPLEMENTATION
#include "huffman.hpp"
#include "lzw.hpp"
#include <boost/program_options.hpp>
#include <ctime>
#include <iostream>
#include <fstream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
namespace po = boost::program_options;

// Advance initializations
struct results {
	double compress_time;
	double decompress_time;
	int compressed_size;
	double compression_rate;
};

void text(string path);
void image(string path);
int filesize(const string path);
int compute_huffman(uint8_t * data, int length, double * compress_time, double * decompress_time);
int compute_lzw(uint8_t * data, int length, double * compress_time, double * decompress_time);
double calcTime(clock_t start, clock_t end);
double calcTaux(int compressedSize, int size);
void output_res(const int * length, const results * huff_res, const results * lzw_res);

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
			text(path);
		}
		else if (type == "img")
		{
			image(path);
		}
	}
	else {
		cout << "You must specify type and path" << endl;
		cout << desc << endl;
		return -2;
	}
	return 0;
}

// Finds inital file size
int filesize(const string path) {
	ifstream in(path, ifstream::ate | ifstream::binary);

	return in.tellg();
}

// Returns time in milliseconds
double calcTime(clock_t start, clock_t end) {
	return double(end - start) * 1000.0 / CLOCKS_PER_SEC;
}

// Returns the compression rate
double calcTaux(int compressedSize, int size) {
	return 1.0 - double(compressedSize)/double(size);
}

// Processes the text file before runing benchmark
void text(string path) {
	cout << "Following output for " << path << endl;
	cout << "---------------------" << endl << endl;

	ifstream inFile;

	inFile.open(path, ifstream::binary);
	if (!inFile) {
		cout << "unable to open" << path << endl;
		exit(1);
	}
	int length = filesize(path);

	char *buffer = new char [length];

	inFile.read(buffer, length);

	if (!inFile) {
		cout << "error: only " << inFile.gcount() << " could be read" << endl;
	}

	// Huffman Compression
	double time_huff_c{0};
	double time_huff_d{0};
	int taille_huff = compute_huffman((uint8_t*) buffer, length, &time_huff_c, &time_huff_d);
	double taux_huff = calcTaux(taille_huff, length);

	// LZW compression
	double time_lzw_c{0};
	double time_lzw_d{0};
	int taille_lzw = compute_lzw((uint8_t*) buffer, length, &time_lzw_c, &time_lzw_d);
	double taux_lzw = calcTaux(taille_lzw, length);

	// Save results in struct
	results huff_res {
		time_huff_c,
		time_huff_d,
		taille_huff,
		taux_huff
	};
	results lzw_res {
		time_lzw_c,
		time_lzw_d,
		taille_lzw,
		taux_lzw
	};

	// Post processing
	output_res(&length, &huff_res, &lzw_res);

	delete[] buffer;

	inFile.close();
}

// Runs huffman benchmark and returns compressed size in bytes
int compute_huffman(uint8_t * data, int length, double * compress_time, double * decompress_time) {
	int compressedSizeBytes = 0;
    int compressedSizeBits  = 0;
	uint8_t * compressedData = nullptr;
	vector<uint8_t> uncompressedBuffer(length, 0);

    // Compress:
	clock_t start_c = clock();
    huffman::easyEncode(data, length, &compressedData, &compressedSizeBytes, &compressedSizeBits);
	clock_t end_c = clock();
	*compress_time = calcTime(start_c, end_c);

	// Decompress
	clock_t start_d = clock();
	const int uncompressedSize = huffman::easyDecode(compressedData, compressedSizeBytes, compressedSizeBits, uncompressedBuffer.data(), uncompressedBuffer.size());
	clock_t end_d = clock();
	*decompress_time = calcTime(start_d, end_d);

	// Validate:
    if (uncompressedSize != length)
    {
        cerr << "HUFFMAN COMPRESSION ERROR! Size mismatch!" << endl;
    }
    if (std::memcmp(uncompressedBuffer.data(), data, length) != 0)
    {
        cerr << "HUFFMAN COMPRESSION ERROR! Data corrupted!" << endl;
    }

	HUFFMAN_MFREE(compressedData);

	// Return compression ratio
	return compressedSizeBytes;
}

// Runs LZW benchmark and returns compressed size in bytes
int compute_lzw(uint8_t * data, int length, double * compress_time, double * decompress_time) {
	int compressedSizeBytes = 0;
    int compressedSizeBits  = 0;
	uint8_t * compressedData = nullptr;
	vector<uint8_t> uncompressedBuffer(length, 0);

    // Compress:
	clock_t start_c = clock();
    lzw::easyEncode(data, length, &compressedData, &compressedSizeBytes, &compressedSizeBits);
	clock_t end_c = clock();
	*compress_time = calcTime(start_c, end_c);

	// Decompress
	clock_t start_d = clock();
	const int uncompressedSize = lzw::easyDecode(compressedData, compressedSizeBytes, compressedSizeBits, uncompressedBuffer.data(), uncompressedBuffer.size());
	clock_t end_d = clock();
	*decompress_time = calcTime(start_d, end_d);

	// Validate:
    if (uncompressedSize != length)
    {
        cerr << "LZW COMPRESSION ERROR! Size mismatch!" << endl;
    }
    if (std::memcmp(uncompressedBuffer.data(), data, length) != 0)
    {
        cerr << "LZW COMPRESSION ERROR! Data corrupted!" << endl;
    }

	LZW_MFREE(compressedData);

	// Return compression ratio
	return compressedSizeBytes;
}

// Prints out benchmark results
void output_res(const int * length, const results * huff_res, const results * lzw_res){
	cout << "Taille du fichier sans compression = " << *length                   << endl;
	cout << "Taille compresser Huffman          = " << huff_res->compressed_size << endl;
	cout << "Taille compresser LZW              = " << lzw_res->compressed_size  << endl;

	cout << endl;

	cout << "Temps de compression Huffman       = " << huff_res->compress_time    << " ms" << endl;
	cout << "Temps de decompression Huffman     = " << huff_res->decompress_time  << " ms" << endl;
	cout << "Taux de compression Huffman        = " << huff_res->compression_rate << endl;

	cout << endl;

	cout << "Temps de compression LZW           = " << lzw_res->compress_time    << " ms" << endl;
	cout << "Temps de decompression LZW         = " << lzw_res->decompress_time  << " ms" << endl;
	cout << "Taux de compression LZW            = " << lzw_res->compression_rate << endl;
}

void image(string path) {
	cout << "Reading image" << endl;

	cv::Mat inImage = cv::imread(path, cv::IMREAD_COLOR);

	uint8_t *sampleData = (uint8_t *)inImage.data;

	int sampleSize = inImage.rows * inImage.cols * 3; // 3 CHANNELS (RGB)
	// uint8_t *compressedData = nullptr;
	// int compressedSizeBytes = 0;
	// int compressedSizeBits = 0;

	// uint8_t *uncompressedData = new uint8_t[sampleSize];
	// int uncompressedSize = 0;

	// =================== Compression Huffman =========================
	double time_huff_c{0};
	double time_huff_d{0};
	int taille_huff = compute_huffman((uint8_t*) sampleData, sampleSize, &time_huff_c, &time_huff_d);
	double taux_huff = calcTaux(taille_huff, sampleSize);

	// Save results in struct
	results huff_res {
		time_huff_c,
		time_huff_d,
		taille_huff,
		taux_huff
	};

	// huffman::easyEncode((uint8_t *)sampleData, sampleSize, &compressedData,
	// 					&compressedSizeBytes, &compressedSizeBits);
	// uncompressedSize = huffman::easyDecode(compressedData, compressedSizeBytes, compressedSizeBits,
	// 									uncompressedData, sampleSize);

	// cout << "Huffman compression results ========" << endl;
	// if (compressedData)
	// {
	// 	cout << "Compression successful" << endl;
	// 	cout << "Bytes size compressed : " << compressedSizeBytes << endl;
	// 	cout << "Bits size compressed : " << compressedSizeBits << endl;
    //
	// 	// Compression validation
	// 	bool successful = true;
	// 	if (uncompressedSize != sampleSize)
	// 	{
	// 		cerr << "HUFFMAN COMPRESSION ERROR! Size mismatch" << endl;
	// 		;
	// 		successful = false;
	// 	}
	// 	if (memcmp(uncompressedData, sampleData, sampleSize) != 0)
	// 	{
	// 		cerr << "HUFFMAN COMPRESSION ERROR! Data corrupted" << endl;
	// 		successful = false;
	// 	}
    //
	// 	if (successful)
	// 	{
	// 		cout << "Huffman compression/decompression lossless" << endl;
    //
	// 		string compPathHuffman = "compression_results/compressedHuffman";
	// 		cout << "Check " << compPathHuffman << " to see the Huffman compression result." << endl;
    //
	// 		ofstream compHuffmanFile;
	// 		compHuffmanFile.open(compPathHuffman);
	// 		compHuffmanFile << compressedData << endl;
	// 		compHuffmanFile.close();
	// 	}
	// } else {
	// 	cout << "Error while compressing" << endl;
	// }

	// string uncompPathHuffman = "compression_results/uncompressedHuffman.png";
	// cout << "Check " << uncompPathHuffman << " to see the decompression result." << endl;
    //
	// cv::Mat outImageHuffman = cv::Mat(inImage.rows, inImage.cols, CV_8UC3, (uchar *)uncompressedData);
	// imwrite(uncompPathHuffman, outImageHuffman);
    //
	// HUFFMAN_MFREE(compressedData);

	// =================== Compression LZW =============================
	double time_lzw_c{0};
	double time_lzw_d{0};
	int taille_lzw = compute_lzw((uint8_t*) sampleData, sampleSize, &time_lzw_c, &time_lzw_d);
	double taux_lzw = calcTaux(taille_lzw, sampleSize);

	// Save results in struct
	results lzw_res {
		time_lzw_c,
		time_lzw_d,
		taille_lzw,
		taux_lzw
	};

	// lzw::easyEncode((uint8_t *)sampleData, sampleSize, &compressedData,
	// 					&compressedSizeBytes, &compressedSizeBits);
	// uncompressedSize = lzw::easyDecode(compressedData, compressedSizeBytes, compressedSizeBits,
	// 									   uncompressedData, sampleSize);
    //
	// cout << "LZW compression results ========" << endl;
	// if (compressedData)
	// {
	// 	cout << "Compression successful" << endl;
	// 	cout << "Bytes size compressed : " << compressedSizeBytes << endl;
	// 	cout << "Bits size compressed : " << compressedSizeBits << endl;
    //
	// 	// Compression validation
	// 	bool successful = true;
	// 	if (uncompressedSize != sampleSize)
	// 	{
	// 		cerr << "LZW COMPRESSION ERROR! Size mismatch" << endl;
	// 		successful = false;
	// 	}
	// 	if (memcmp(uncompressedData, sampleData, sampleSize) != 0)
	// 	{
	// 		cerr << "LZW COMPRESSION ERROR! Data corrupted" << endl;
	// 		successful = false;
	// 	}
    //
	// 	if (successful)
	// 	{
	// 		cout << "LZW compression/decompression lossless" << endl;
    //
	// 		string compPathLZW = "compression_results/compressedLZW";
	// 		cout << "Check " << compPathLZW << " to see the LZW compression result." << endl;
    //
	// 		ofstream compHuffmanFile;
	// 		compHuffmanFile.open(compPathLZW);
	// 		compHuffmanFile << compressedData << endl;
	// 		compHuffmanFile.close();
	// 	}
	// }
	// else
	// {
	// 	cout << "Error while compressing" << endl;
	// }
    //
	// string uncompPathLZW = "compression_results/uncompressedLZW.png";
	// cout << "Check " << uncompPathLZW << " to see the decompression result." << endl;
    //
	// cv::Mat outImageLZW = cv::Mat(inImage.rows, inImage.cols, CV_8UC3, (uchar *)uncompressedData);
	// imwrite(uncompPathLZW, outImageLZW);

	// cv::Mat noise = cv::Mat(outImageLZW.size(), CV_8UC3);
	// cv::randn(noise, 32, 4);

	// cv::Mat imageBruitee = outImageLZW + noise;

	// imwrite("lena_bruit.png", imageBruitee);

	// =================== Post Processing =========================
	output_res(&sampleSize, &huff_res, &lzw_res);

	delete[] sampleData;
	// LZW_MFREE(compressedData);
}
