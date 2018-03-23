#include "pipeline.hpp"

namespace fs = boost::filesystem;
using namespace std;

const int HIST_SIZE = 256;

int open(fs::path path){
	// open the video file
	cv::VideoCapture vid(path.string());
	// check if opening video succeded
	if (!vid.isOpened()) {
		cout << "Failed to open " << path.string() << endl;
		return -1;
	}

	std::vector<cv::Mat> b_hist, g_hist, r_hist;
	std::vector<double> dist_b, dist_g, dist_r;

	histogrammes(vid, &b_hist, &g_hist, &r_hist);

	return 0;
}

void histogrammes(cv::VideoCapture& vid, std::vector<cv::Mat>* b_hist, std::vector<cv::Mat>* g_hist, std::vector<cv::Mat>* r_hist) {
	// Cherche numero de frames dans la video
	int frameNumber = (int) vid.get(CV_CAP_PROP_FRAME_COUNT);
	cout << "The number of frames in this file is = " << frameNumber << endl;

	// calcule tout les histogrammes
	for (int i = 0; i < frameNumber - 1; i++) {
		cv::Mat frame;
		vid >> frame; // Get the next frame from the video
		if (frame.empty()) {
			cout << "The program finished prematurely at frame " << i+1 << endl;
			break;
		}

		// split la video dans les trois caneaux
		std::vector<cv::Mat> bgr_planes;
		split(frame, bgr_planes);

		float range[] = { 0, HIST_SIZE };
		const float* histRange = { range };

		bool uniform = true;
		bool accumulate = false;

		cv:: Mat b_hist_temp, g_hist_temp, r_hist_temp;

		// calcule les histogrammes
		cv::calcHist(&bgr_planes[0], 1, 0, cv::Mat(), b_hist_temp, 1, &HIST_SIZE, &histRange, uniform, accumulate);
		cv::calcHist(&bgr_planes[1], 1, 0, cv::Mat(), g_hist_temp, 1, &HIST_SIZE, &histRange, uniform, accumulate);
		cv::calcHist(&bgr_planes[2], 1, 0, cv::Mat(), r_hist_temp, 1, &HIST_SIZE, &histRange, uniform, accumulate);

		// sauvegarde histogrammes
		b_hist -> push_back(b_hist_temp);
		g_hist -> push_back(g_hist_temp);
		r_hist -> push_back(r_hist_temp);
	}
}
