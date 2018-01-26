#define HUFFMAN_IMPLEMENTATION
#define LZW_IMPLEMENTATION
#include "huffman.hpp"
#include "lzw.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;

void text(string path);
void image(string path);

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
	else if (type == "img") {
		image(path);
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
	// check test.cpp in compression
	uint8_t * cBuffer = nullptr;
	int compressedSizeBytes = 0;
	int compressedSizeBits = 0;
	huffman::easyEncode((uint8_t*) buffer, length, &cBuffer,
						&compressedSizeBytes, &compressedSizeBits);

	if (cBuffer) {
		cout << "compressed successfully" << endl;
	} else {
		cout << "ERROR: not compressed successfully" << endl;
	}
	// TODO: calculate time of compression
	// TODO: calculate compression ratio

	string compPathHuffman = "compression_results/compressedHuffman";
	cout << "Check " << compPathHuffman << " to see the Huffman compression result." << endl;

	delete[] buffer;
	HUFFMAN_MFREE(cBuffer);

	inFile.close();
	cout << path << endl;
}

void image(string path) {
	cout << "Reading image" << endl;

	cv::Mat inImage = cv::imread(path, cv::IMREAD_COLOR);

	uint8_t *sampleData = (uint8_t *)inImage.data;

	int sampleSize = inImage.rows * inImage.cols * 3; // 3 CHANNELS (RGB)
	uint8_t *compressedData = nullptr;
	int compressedSizeBytes = 0;
	int compressedSizeBits = 0;

	uint8_t *uncompressedData = new uint8_t[sampleSize];
	int uncompressedSize = 0;

	// =================== Compression Huffman =========================
	huffman::easyEncode((uint8_t *)sampleData, sampleSize, &compressedData,
						&compressedSizeBytes, &compressedSizeBits);
	uncompressedSize = huffman::easyDecode(compressedData, compressedSizeBytes, compressedSizeBits,
										uncompressedData, sampleSize);

	cout << "Huffman compression results ========" << endl;
	if (compressedData)
	{
		cout << "Compression successful" << endl;
		cout << "Bytes size compressed : " << compressedSizeBytes << endl;
		cout << "Bits size compressed : " << compressedSizeBits << endl;

		// Compression validation
		bool successful = true;
		if (uncompressedSize != sampleSize)
		{
			cerr << "HUFFMAN COMPRESSION ERROR! Size mismatch" << endl;
			;
			successful = false;
		}
		if (memcmp(uncompressedData, sampleData, sampleSize) != 0)
		{
			cerr << "HUFFMAN COMPRESSION ERROR! Data corrupted" << endl;
			successful = false;
		}

		if (successful)
		{
			cout << "Huffman compression/decompression lossless" << endl;

			string compPathHuffman = "compression_results/compressedHuffman";
			cout << "Check " << compPathHuffman << " to see the Huffman compression result." << endl;

			ofstream compHuffmanFile;
			compHuffmanFile.open(compPathHuffman);
			compHuffmanFile << compressedData << endl;
			compHuffmanFile.close();
		}
	} else {
		cout << "Error while compressing" << endl;
	}

	string uncompPathHuffman = "compression_results/uncompressedHuffman.png";
	cout << "Check " << uncompPathHuffman << " to see the decompression result." << endl;

	cv::Mat outImageHuffman = cv::Mat(inImage.rows, inImage.cols, CV_8UC3, (uchar *)uncompressedData);
	imwrite(uncompPathHuffman, outImageHuffman);

	HUFFMAN_MFREE(compressedData);

	// =================== Compression LZW =============================
	lzw::easyEncode((uint8_t *)sampleData, sampleSize, &compressedData,
						&compressedSizeBytes, &compressedSizeBits);
	uncompressedSize = lzw::easyDecode(compressedData, compressedSizeBytes, compressedSizeBits,
										   uncompressedData, sampleSize);

	cout << "LZW compression results ========" << endl;
	if (compressedData)
	{
		cout << "Compression successful" << endl;
		cout << "Bytes size compressed : " << compressedSizeBytes << endl;
		cout << "Bits size compressed : " << compressedSizeBits << endl;

		// Compression validation
		bool successful = true;
		if (uncompressedSize != sampleSize)
		{
			cerr << "LZW COMPRESSION ERROR! Size mismatch" << endl;
			successful = false;
		}
		if (memcmp(uncompressedData, sampleData, sampleSize) != 0)
		{
			cerr << "LZW COMPRESSION ERROR! Data corrupted" << endl;
			successful = false;
		}

		if (successful)
		{
			cout << "LZW compression/decompression lossless" << endl;

			string compPathLZW = "compression_results/compressedLZW";
			cout << "Check " << compPathLZW << " to see the LZW compression result." << endl;

			ofstream compHuffmanFile;
			compHuffmanFile.open(compPathLZW);
			compHuffmanFile << compressedData << endl;
			compHuffmanFile.close();
		}
	}
	else
	{
		cout << "Error while compressing" << endl;
	}

	string uncompPathLZW = "compression_results/uncompressedLZW.png";
	cout << "Check " << uncompPathLZW << " to see the decompression result." << endl;

	cv::Mat outImageLZW = cv::Mat(inImage.rows, inImage.cols, CV_8UC3, (uchar *)uncompressedData);
	imwrite(uncompPathLZW, outImageLZW);

	// cv::Mat noise = cv::Mat(outImageLZW.size(), CV_8UC3);
	// cv::randn(noise, 32, 4);

	// cv::Mat imageBruitee = outImageLZW + noise;

	// imwrite("lena_bruit.png", imageBruitee);

	delete[] uncompressedData;
	LZW_MFREE(compressedData);
}