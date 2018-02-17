#include "pipeline.hpp"

using namespace std;

int code(fs::path path, fs::path save, bool subsampling) {
	save /= path.filename();
	save.replace_extension(".jpeg");

	cout << "Will read file: " << path.string()
		 << " and save its compressed form as " << save.string() << endl;

	// Load image in
	cv::Mat bgr = cv::imread(path.string(), CV_LOAD_IMAGE_COLOR);
	cv::Mat_<uchar> y, cb, cr;

	if(! bgr.data)
    {
        cout <<  "Could not open or find the image" << endl ;
        return -1;
    }
	else if (bgr.rows % 8 != 0 && bgr.cols % 8 != 0)
	{
		cout << "Height and width of the image must be multiples of 8" << endl;
	}
	// cout << "Height of the image " << bgr.rows << ", width of the image " << bgr.cols << endl;

	dispImg("Original image", bgr);

	// convert BGR to YCbCr
	bgr2ycbcr(bgr, y, cb, cr, subsampling);
	if(!y.data || !cb.data || !cr.data)
    {
        cout <<  "Could not convert the image to YCbCr" << endl ;
        return -1;
	}
	std::vector<cv::Mat_<uchar>> blocksY = matrix2blocks(y);
	std::vector<cv::Mat_<uchar>> blocksCb = matrix2blocks(cb);
	std::vector<cv::Mat_<uchar>> blocksCr = matrix2blocks(cr);

	std::vector<cv::Mat_<float>> dctY = blocks2dct(blocksY);
	std::vector<cv::Mat_<float>> dctCb = blocks2dct(blocksCb);
	std::vector<cv::Mat_<float>> dctCr = blocks2dct(blocksCr);

	std::vector<cv::Mat_<signed char>> quantifY = quantification(dctY);
	std::vector<cv::Mat_<signed char>> quantifCb = quantification(dctCb);
	std::vector<cv::Mat_<signed char>> quantifCr = quantification(dctCr);

	std::vector<cv::Mat_<float>> inv_quantifY = inv_quantification(quantifY);
	std::vector<cv::Mat_<float>> inv_quantifCb = inv_quantification(quantifCb);
	std::vector<cv::Mat_<float>> inv_quantifCr = inv_quantification(quantifCr);

	std::vector<cv::Mat_<uchar>> blocksY2 = dct2blocks(inv_quantifY);
	std::vector<cv::Mat_<uchar>> blocksCb2 = dct2blocks(inv_quantifCb);
	std::vector<cv::Mat_<uchar>> blocksCr2 = dct2blocks(inv_quantifCr);

	cv::Mat_<uchar> y2 = blocks2matrix(blocksY2, y.size());
	cv::Mat_<uchar> cb2 = blocks2matrix(blocksCb2, cb.size());
	cv::Mat_<uchar> cr2 = blocks2matrix(blocksCr2, cr.size());

	// dispImg("Y", y);
	// dispImg("Cb", cb);
	// dispImg("Cr", cr);

	// dispImg("Y after DCT", y2);
	// dispImg("Cb after DCT", cb2);
	// dispImg("Cr after DCT", cr2);

	cv::Mat bgrOut = ycbcr2bgr(y2, cb2, cr2);

	dispImg("Decompression result", bgrOut);

	if(subsampling)
	{
		cv::Mat diff = bgr-bgrOut;
		dispImg("Differences original/decompressed", diff);
		// cv::Mat diffNorm;
		// cv::normalize(bgr-bgrOut, diffNorm, 0.f, 255.f, cv::NORM_MINMAX);
		// dispImg("Subsampling differences (normalized min-max)", diffNorm);

		// cv::Mat diffBlocks;
		// cv::normalize(y - y2, diffBlocks, 0.f, 255.f, cv::NORM_MINMAX);
		// dispImg("Differences with blocks", diffBlocks);
	}

	// Wait for a keystroke in the window
	cv::waitKey(0);
	return 0;
}

int decode(fs::path path, fs::path save) {
	cout << "Will read compressed file: " << path.string()
		 << " and display it" << endl;

	return 0;
}

void dispImg(string message, cv::Mat &image) {
	// Create a window for display.
	cv::namedWindow( message, cv::WINDOW_AUTOSIZE );

	// Show our image inside it.
	cv::imshow( message, image );
}
