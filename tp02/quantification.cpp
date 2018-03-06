#include "quantification.hpp"
#include <iostream>

const cv::Mat_<int> basicQuantif = (cv::Mat_<int>(8, 8) << 16, 11, 10, 16, 24, 40, 51, 61,
                                      12, 12, 14, 19, 26, 58, 60, 55,
                                      14, 13, 16, 24, 40, 57, 69, 56,
                                      14, 17, 22, 29, 51, 87, 80, 62,
                                      18, 22, 37, 56, 68, 109, 103, 77,
                                      24, 35, 55, 64, 81, 104, 113, 92,
                                      49, 64, 78, 87, 103, 121, 120, 101,
                                      72, 92, 95, 98, 112, 100, 103, 99);

const cv::Mat_<int> quantifConstant = (cv::Mat_<int>(8,8) << 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1);

const cv::Mat_<int> quantifTriDiag = (cv::Mat_<int>(8,8) << 1, 1, 0, 0, 0, 0, 0, 0,
1, 1, 1, 0, 0, 0, 0, 0,
0, 1, 1, 1, 0, 0, 0, 0,
0, 0, 1, 1, 1, 0, 0, 0,
0, 0, 0, 1, 1, 1, 0, 0,
0, 0, 0, 0, 1, 1, 1, 0,
0, 0, 0, 0, 0, 1, 1, 1,
0, 0, 0, 0, 0, 0, 1, 1);

std::vector<cv::Mat_<char>> quantification(const std::vector<cv::Mat_<float>> &inDct, quantifType type, int level)
{
    cv::Mat_<int> quantifLevel;
	switch (type) {
		case quantifType::BASIC :
			std::cout << "JPEG Quantification" << std::endl;
			quantifLevel = basicQuantif * level;
			std::cout << quantifLevel << std::endl;
			break;
		case quantifType::LEVEL :
			std::cout << "Quantification level : " << level << std::endl;
			quantifLevel = createQuantification(level);
			std::cout << quantifLevel << std::endl;
			break;
		case quantifType::CONSTANT :
			std::cout << "Constant quantification : " << level << std::endl;
			quantifLevel = quantifConstant * level;
			std::cout << quantifLevel << std::endl;
			break;
		case quantifType::TRI_DIAGONAL :
			std::cout << "Tri-diagonal quantification : " << level << std::endl;
			quantifLevel = quantifTriDiag * level;
			std::cout << quantifLevel << std::endl;
			break;
		default :
			std::cout << "JPEG Quantification" << std::endl;
			quantifLevel = basicQuantif * level;
			std::cout << quantifLevel << std::endl;
			break;
	}

    int nbBlocks = inDct.size();
    std::vector<cv::Mat_<char>> outQuantif;
    outQuantif.reserve(nbBlocks);

    for (int i = 0; i < nbBlocks; i++)
    {
        cv::Mat_<float> inBlock = inDct[i];
        cv::Mat_<char> quantif(inBlock.size());
        int rows = inBlock.rows;
        int cols = inBlock.cols;
        for (int u = 0; u < rows; u++)
        {
            for (int v = 0; v < cols; v++)
            {
                float tmp = std::nearbyint(inBlock.at<float>(u, v) / quantifLevel.at<int>(u, v));
                quantif.at<char>(u, v) = tmp;
                // if(tmp > 127 || tmp < -128)
                //     std::cout << "Overflow" << std::endl;
            }
        }
        outQuantif.push_back(quantif);
    }
    return outQuantif;
}

std::vector<cv::Mat_<float>> inv_quantification(const std::vector<cv::Mat_<char>> &inQuantif, quantifType type, int level)
{
    cv::Mat_<int> quantifLevel;
	switch (type) {
		case quantifType::BASIC :
			quantifLevel = basicQuantif * level;
			break;
		case quantifType::LEVEL :
			quantifLevel = createQuantification(level);
			break;
		case quantifType::CONSTANT :
			quantifLevel = quantifConstant * level;
			break;
		case quantifType::TRI_DIAGONAL :
			quantifLevel = quantifTriDiag * level;
			break;
		default :
			quantifLevel = basicQuantif * level;
			break;
	}

    int nbBlocks = inQuantif.size();
    std::vector<cv::Mat_<float>> outBlocks;
    outBlocks.reserve(nbBlocks);

    for (int i = 0; i < nbBlocks; i++)
    {
        cv::Mat_<char> inQuant = inQuantif[i];
        cv::Mat_<float> block(inQuant.size());
        int rows = inQuant.rows;
        int cols = inQuant.cols;
        for (int u = 0; u < rows; u++)
        {
            for (int v = 0; v < cols; v++)
            {
                block.at<float>(u, v) = inQuant.at<char>(u, v) * quantifLevel.at<int>(u, v);
            }
        }
        outBlocks.push_back(block);
    }
    return outBlocks;
}

cv::Mat_<int> createQuantification(int level)
{
    cv::Size sizeQuantif(8,8);
    cv::Mat_<int> quantif(sizeQuantif);

    for(int i = 0; i < quantif.rows; i++)
    {
        for (int j = 0; j < quantif.cols; j++)
        {
            quantif.at<int>(i,j) = 1 + (i+j+1) * level;
        }
    }

    return quantif;
}
