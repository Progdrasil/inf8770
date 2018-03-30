#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include <boost/filesystem.hpp>
#include <opencv2/opencv.hpp>
#include <math.h>

int open(boost::filesystem::path path);

void histogrammes(cv::VideoCapture& vid, std::vector<cv::Mat>& b_hist, std::vector<cv::Mat>& g_hist, std::vector<cv::Mat>& r_hist);
float calculerDistance(std::vector<cv::Mat>& b_hist, std::vector<cv::Mat>& g_hist, std::vector<cv::Mat>& r_hist, int indexSrc, int indexCmp);
void dispImg(std::string message, cv::Mat &image);

#endif
