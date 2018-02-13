#include "pipeline.hpp"

using namespace std;

void code(fs::path path, fs::path save) {
	save /= path.filename();
	save.replace_extension(".jpeg");

	cout << "Will read file: " << path.string()
		 << " and save its compressed form as " << save.string() << endl;

	cv::Mat bgr = cv::imread(path.string(), CV_LOAD_IMAGE_COLOR);
}

void decode(fs::path path, fs::path save) {
	cout << "Will read compressed file: " << path.string()
		 << " and display it" << endl;
}
