#include "pipeline.hpp"

using namespace std;

int code(fs::path path, fs::path save) {
	save /= path.filename();
	save.replace_extension(".jpeg");

	cout << "Will read file: " << path.string()
		 << " and save its compressed form as " << save.string() << endl;

	// Load image in
	cv::Mat bgr = cv::imread(path.string(), CV_LOAD_IMAGE_COLOR);
	cv::Mat y, cb, cr;

	if(! bgr.data)
    {
        cout <<  "Could not open or find the image" << endl ;
        return -1;
    }

	dispImg("Original image", bgr);

	// convert BGR to YCbCr
	bgr2ycbcr(bgr, y, cb, cr);
	if(!y.data || !cb.data || !cr.data)
    {
        cout <<  "Could not convert the image to YCbCr" << endl ;
        return -1;
    }

	dispImg("Y", y);
	dispImg("Cb", cb);
	dispImg("Cr", cr);

	return 0;
}

int decode(fs::path path, fs::path save) {
	cout << "Will read compressed file: " << path.string()
		 << " and display it" << endl;

	return 0;
}

void dispImg(string &message, cv::Mat &image) {
	// Create a window for display.
	cv::namedWindow( message, cv::WINDOW_AUTOSIZE );

	// Show our image inside it.
	cv::imshow( message, image );

	// Wait for a keystroke in the window
	cv::waitKey(0);
}
