#include "pipeline.cpp"

namespace fs = boost::filesystem;
using namespace std;

int open(fs::path path){
	// open the video file
	cv::VideoCapture vid(path.string());
	// check if opening video succeded
	if (!vid.isOpened()) {
		cout << "Failed to open " << path.string() << endl;
		return -1;
	}

	cv::Mat edges;
	return 0
}
