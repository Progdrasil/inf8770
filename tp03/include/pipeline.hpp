#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include <boost/filesystem.hpp>
#include <opencv2/opencv.hpp>

int open(boost::filesystem::path path);

void histogrammes(cv::VideoCapture& vid, std::vector<cv::Mat>* b_hist, std::vector<cv::Mat>* g_hist, std::vector<cv::Mat>* r_hist);

#endif
