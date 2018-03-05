
#define RLE_IMPLEMENTATION
#define HUFFMAN_IMPLEMENTATION
#include "rle.hpp"
#include "huffman.hpp"
#include "pipeline.hpp"

using namespace std;

int code(fs::path path, bool subsampling, uint quantifLevel, uint8_t **huffmanData, std::vector<cv::Size> &ycbcrSize, std::vector<uint> &lineSizes, std::vector<int> &compressionData)
{
	cout << "Will read file: " << path.string() << endl;

	// Load image in
	cv::Mat bgr = cv::imread(path.string(), CV_LOAD_IMAGE_COLOR);
	cv::Mat_<uchar> y, cb, cr;

	if (!bgr.data)
	{
		cout << "Could not open or find the image" << endl;
		return -1;
	}
	else if (bgr.rows % 8 != 0 && bgr.cols % 8 != 0)
	{
		cout << "Height and width of the image must be multiples of 8" << endl;
	}

	dispImg("Original image", bgr);
	cout << "Original image size bytes = " << bgr.total() * bgr.elemSize() << endl;


	// convert BGR to YCbCr
	bgr2ycbcr(bgr, y, cb, cr, subsampling);
	if (!y.data || !cb.data || !cr.data)
	{
		cout << "Could not convert the image to YCbCr" << endl;
		return -1;
	}
	ycbcrSize.push_back(y.size());
	ycbcrSize.push_back(cb.size());
	ycbcrSize.push_back(cr.size());

	std::vector<cv::Mat_<uchar>> blocksY = matrix2blocks(y);
	std::vector<cv::Mat_<uchar>> blocksCb = matrix2blocks(cb);
	std::vector<cv::Mat_<uchar>> blocksCr = matrix2blocks(cr);

	std::vector<cv::Mat_<uchar>> blocks = linearise(blocksY, blocksCb, blocksCr, lineSizes);

	std::vector<cv::Mat_<float>> dct = blocks2dct(blocks);

	std::vector<cv::Mat_<char>> quantif = quantification(dct, quantifLevel);
	compressionData.push_back(quantif.size());

	std::vector<char> zigzag = blocks2vector(quantif);

	// Compress with RLE:
	int zigzagSize = zigzag.size();
	const uint8_t *zigzagData = (uint8_t *)zigzag.data();
	std::vector<std::uint8_t> rle(zigzagSize * 4, 0); // RLE might make things bigger.

	const int rleSize = rle::easyEncode(zigzagData, zigzagSize,
										rle.data(),
										rle.size());

	compressionData.push_back(zigzagSize);
	compressionData.push_back(rleSize);

	std::cout << "RLE compressed size bytes   = " << rleSize << "\n";
	std::cout << "RLE uncompressed size bytes = " << zigzagSize << "\n";

	// Compress with Huffman:
	int huffmanSizeBytes = 0;
	int huffmanSizeBits = 0;
	uint8_t *rleData = rle.data();

	huffman::easyEncode(rleData, rleSize, huffmanData,
						&huffmanSizeBytes, &huffmanSizeBits);
	compressionData.push_back(huffmanSizeBytes);
	compressionData.push_back(huffmanSizeBits);

	std::cout << "Huffman compressed size bytes   = " << huffmanSizeBytes << "\n";
	std::cout << "Huffman uncompressed size bytes = " << rleSize << "\n";

	return 0;
}

int decode(uint8_t **huffmanData, uint quantifLevel, std::vector<cv::Size> &ycbcrSize, std::vector<uint> &lineSizes, std::vector<int> &compressionData)
{
	int huffmanSizeBits = compressionData.back();
	compressionData.pop_back();
	int huffmanSizeBytes = compressionData.back();
	compressionData.pop_back();
	int rleSize = compressionData.back();
	compressionData.pop_back();
	int zigzagSize = compressionData.back();
	compressionData.pop_back();
	int quantifSize = compressionData.back();
	compressionData.pop_back();

	// Restore from Huffman:
	std::vector<std::uint8_t> uncompressedHuffman(rleSize, 0);
	const int uncompressedSizeHuffman = huffman::easyDecode(*huffmanData, huffmanSizeBytes, huffmanSizeBits,
															uncompressedHuffman.data(), uncompressedHuffman.size());

	// Restore from RLE:
	std::vector<std::uint8_t> uncompressedRLE(zigzagSize, 0);
	const int uncompressedSizeRLE = rle::easyDecode(uncompressedHuffman.data(), rleSize,
													uncompressedRLE.data(), uncompressedRLE.size());
	std::vector<char> inv_rle(uncompressedRLE.begin(), uncompressedRLE.end());

	// TODO Changer hardcode de la taille
	cv::Size sizeblock(8,8);
	std::vector<cv::Mat_<char>> inv_zigzag = vector2blocks(inv_rle, sizeblock, quantifSize);
	std::vector<cv::Mat_<float>> inv_quantif = inv_quantification(inv_zigzag, quantifLevel);

	std::vector<cv::Mat_<uchar>> blocks = dct2blocks(inv_quantif);

	std::vector<cv::Mat_<uchar>> blocksY, blocksCb, blocksCr;
	delinearise(blocks, blocksY, blocksCb, blocksCr, lineSizes);

	cv::Mat_<uchar> y = blocks2matrix(blocksY, ycbcrSize.at(0));
	cv::Mat_<uchar> cb = blocks2matrix(blocksCb, ycbcrSize.at(1));
	cv::Mat_<uchar> cr = blocks2matrix(blocksCr, ycbcrSize.at(2));

	cv::Mat bgrOut = ycbcr2bgr(y, cb, cr);

	dispImg("Decompression result", bgrOut);

	// Wait for a keystroke in the window
	cv::waitKey(0);

	return 0;
}

void dispImg(string message, cv::Mat &image)
{
	// Create a window for display.
	cv::namedWindow(message, cv::WINDOW_AUTOSIZE);

	// Show our image inside it.
	cv::imshow(message, image);
}

template <typename T>
std::vector<T> linearise(std::vector<T> &y, std::vector<T> &cb, std::vector<T> &cr, vector<uint> &lineSizes)
{
	std::vector<T> line;

	lineSizes.push_back((uint)y.size());
	line.insert(line.end(), y.begin(), y.end());

	lineSizes.push_back((uint)cb.size());
	line.insert(line.end(), cb.begin(), cb.end());

	lineSizes.push_back((uint)cr.size());
	line.insert(line.end(), cr.begin(), cr.end());

	return line;
}

template <typename T>
void delinearise(std::vector<T> &line, std::vector<T> &y, std::vector<T> &cb, std::vector<T> &cr, std::vector<uint> &lineSizes)
{
	uint ys = lineSizes.at(0);
	uint cbs = lineSizes.at(1);
	uint crs = lineSizes.at(2);

	for (uint i = 0; i < ys; i++)
	{
		y.push_back(line.at(i));
	}

	for (uint i = ys; i < ys + cbs; i++)
	{
		cb.push_back(line.at(i));
	}

	for (uint i = ys + cbs; i < ys + cbs + crs; i++)
	{
		cr.push_back(line.at(i));
	}
}
