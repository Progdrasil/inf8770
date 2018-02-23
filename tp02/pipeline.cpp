#include "pipeline.hpp"

using namespace std;

int code(fs::path path, bool subsampling, uint quantifLevel, std::vector<cv::Mat_<char>> * quantif, std::vector<cv::Size> * ycbcrSize, std::vector<uint> * lineSizes)
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

	// convert BGR to YCbCr
	bgr2ycbcr(bgr, y, cb, cr, subsampling);
	if (!y.data || !cb.data || !cr.data)
	{
		cout << "Could not convert the image to YCbCr" << endl;
		return -1;
	}
	ycbcrSize->push_back(y.size());
	ycbcrSize->push_back(cb.size());
	ycbcrSize->push_back(cr.size());

	std::vector<cv::Mat_<uchar>> blocksY = matrix2blocks(y);
	std::vector<cv::Mat_<uchar>> blocksCb = matrix2blocks(cb);
	std::vector<cv::Mat_<uchar>> blocksCr = matrix2blocks(cr);

	std::vector<cv::Mat_<uchar>> blocks = linearise(blocksY, blocksCb, blocksCr, lineSizes);

	std::vector<cv::Mat_<float>> dct = blocks2dct(blocks);

	*quantif = quantification(dct, quantifLevel);

	// std::vector<int> zigzag = blocks2std::vector(quantif);

	return 0;
}

int decode(std::vector<cv::Mat_<char>> quantif, uint quantifLevel, std::vector<cv::Size> ycbcrSize, std::vector<uint> lineSizes)
{
	std::vector<cv::Mat_<float>> inv_quantif = inv_quantification(quantif, quantifLevel);

	std::vector<cv::Mat_<uchar>> blocks = dct2blocks(inv_quantif);

	std::vector<cv::Mat_<uchar>> blocksY;
	std::vector<cv::Mat_<uchar>> blocksCb;
	std::vector<cv::Mat_<uchar>> blocksCr;
	delinearise(blocks, &blocksY, &blocksCb, &blocksCr, lineSizes);

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

template <typename T> std::vector<T> linearise(std::vector<T> y, std::vector<T> cb, std::vector<T> cr, vector<uint> * lineSizes) {
	std::vector<T> line;

	lineSizes -> push_back((uint)y.size());
	line.insert(line.end(), y.begin(), y.end());

	lineSizes -> push_back((uint)cb.size());
	line.insert(line.end(), cb.begin(), cb.end());

	lineSizes -> push_back((uint)cr.size());
	line.insert(line.end(), cr.begin(), cr.end());

	return line;
}

template <typename T> void delinearise(std::vector<T> line, std::vector<T> * y, std::vector<T> * cb, std::vector<T> * cr, std::vector<uint> lineSizes) {
	uint ys = lineSizes.at(0);
	uint cbs = lineSizes.at(1);
	uint crs = lineSizes.at(2);

	for (uint i = 0; i < ys; i++) {
		y -> push_back(line.at(i));
	}

	for (uint i = ys; i < ys+cbs; i++) {
		cb -> push_back(line.at(i));
	}

	for (uint i = ys+cbs; i < ys+cbs+crs; i++) {
		cr -> push_back(line.at(i));
	}
}
