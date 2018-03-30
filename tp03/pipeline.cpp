#include "pipeline.hpp"

namespace fs = boost::filesystem;
using namespace std;

// number of bins of the histograms
const int HIST_SIZE = 64;
const int CUT_THRESHOLD = 50000;
const int DISSOLVE_THRESHOLD = 5000;
const int WAIT_DISPLAY = 250;

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

	histogrammes(vid, b_hist, g_hist, r_hist);

	std::ofstream fileDist("distances.csv");

	std::cout << "Ouverture du fichier " << std::endl;
	if (!fileDist.is_open())
	{
		std::cerr << "Erreur : impossible d'ouvrir le fichier";
		exit(1);
	}
	
	fileDist << "Frame\tDistance;" << std::endl;

	int frameNumber = (int) vid.get(CV_CAP_PROP_FRAME_COUNT);
	double fps = vid.get(CV_CAP_PROP_FPS);

	for(int i=0;i<frameNumber-2;i++)
	{
		bool dissolveDetected = false;
		int dissolveBegin = i;

		float dist = calculerDistance(b_hist, g_hist, r_hist, i, i+1);
		fileDist << i << "\t" << dist << ";" << std::endl;
		if(dist > CUT_THRESHOLD)
		{
			std::cout << "Coupure detectée entre trames " << i << " et " << i+1 << std::endl;
			std::cout << "Temps de la vidéo : " << (i+1)/fps << "s" << std::endl;
			vid.set(1, i);
			cv::Mat frame;
			vid >> frame;

			dispImg("Coupure - Trame n°"+std::to_string(i), frame);
		}
		else if(dist > DISSOLVE_THRESHOLD)
		{
			while(dist > DISSOLVE_THRESHOLD)
			{
				i++;
				dist = calculerDistance(b_hist, g_hist, r_hist, i, i+1);
				fileDist << i << "\t" << dist << ";" << std::endl;
			}

			float distanceDissolve = calculerDistance(b_hist, g_hist, r_hist, dissolveBegin, i);
			if(distanceDissolve > CUT_THRESHOLD)
			{
				std::cout << "Fondu detecté entre trames " << dissolveBegin << " et " << i << std::endl;
				std::cout << "Temps de la vidéo : " << dissolveBegin/fps << "s et "
					<< i/fps << "s." << std::endl;
			}
		}
	}
	fileDist.close();

	return 0;
}

void histogrammes(cv::VideoCapture& vid, std::vector<cv::Mat>& b_hist, std::vector<cv::Mat>& g_hist, std::vector<cv::Mat>& r_hist) {
	// Cherche numero de frames dans la video
	int frameNumber = (int) vid.get(CV_CAP_PROP_FRAME_COUNT);
	cout << "The number of frames in this file is = " << frameNumber << endl;

	// calcule tout les histogrammes
	for (int i = 0; i < frameNumber - 1 ; i++)
	{
		cv::Mat frame;
		vid >> frame; // Get the next frame from the video
		if (frame.empty()) {
			cout << "The program finished prematurely at frame " << i+1 << endl;
			break;
		}

		// split la video dans les trois caneaux
		std::vector<cv::Mat> bgr_planes;
		split(frame, bgr_planes);

		// Range of RGB values
		float range[] = { 0, 256 };
		const float* histRange = { range };

		bool uniform = true;
		bool accumulate = false;

		cv::Mat b_hist_temp, g_hist_temp, r_hist_temp;

		// calcule les histogrammes
		cv::calcHist(&bgr_planes[0], 1, 0, cv::Mat(), b_hist_temp, 1, &HIST_SIZE, &histRange, uniform, accumulate);
		cv::calcHist(&bgr_planes[1], 1, 0, cv::Mat(), g_hist_temp, 1, &HIST_SIZE, &histRange, uniform, accumulate);
		cv::calcHist(&bgr_planes[2], 1, 0, cv::Mat(), r_hist_temp, 1, &HIST_SIZE, &histRange, uniform, accumulate);

		// std::cout << "Number of elements :" << std::endl;
		// std::cout << frame.rows * frame.cols << std::endl;
		// std::cout << "Hist elements :" << std::endl;
		// std::cout << cv::sum( b_hist_temp )[0] << std::endl;
		// std::cout <<  std::endl;

		// sauvegarde histogrammes
		b_hist.push_back(b_hist_temp);
		g_hist.push_back(g_hist_temp);
		r_hist.push_back(r_hist_temp);
	}
}


float calculerDistance(std::vector<cv::Mat>& b_hists, std::vector<cv::Mat>& g_hists, std::vector<cv::Mat>& r_hists, int indexSrc, int indexCmp)
{
	float distance = 0;

	cv::Mat b_hist_src = b_hists[indexSrc];
	cv::Mat b_hist_cmp = b_hists[indexCmp];

	cv::Mat g_hist_src = g_hists[indexSrc];
	cv::Mat g_hist_cmp = g_hists[indexCmp];

	cv::Mat r_hist_src = r_hists[indexSrc];
	cv::Mat r_hist_cmp = r_hists[indexCmp];

	for(int i = 0; i < b_hists[0].rows * b_hists[0].cols; i++)
	{
		distance += (b_hist_src.at<float>(i) - b_hist_cmp.at<float>(i)) * (b_hist_src.at<float>(i) - b_hist_cmp.at<float>(i));
		distance += (g_hist_src.at<float>(i) - g_hist_cmp.at<float>(i)) * (g_hist_src.at<float>(i) - g_hist_cmp.at<float>(i));
		distance += (r_hist_src.at<float>(i) - r_hist_cmp.at<float>(i)) * (r_hist_src.at<float>(i) - r_hist_cmp.at<float>(i));
	}
	
	return sqrt(distance);
}

void dispImg(string message, cv::Mat &image)
{
	// Create a window for display.
	cv::namedWindow(message, cv::WINDOW_AUTOSIZE);

	// Show our image inside it.
	cv::imshow(message, image);

	cv::waitKey(WAIT_DISPLAY);
	cv::destroyWindow(message);
}
