#include "quantification.hpp"

const cv::Mat_<uchar> quantifMat = (cv::Mat_<uchar>(8, 8) <<
    16, 11, 10, 16, 24, 40, 51, 61,
    12, 12, 14, 19, 26, 58, 60, 55,
    14, 13, 16, 24, 40, 57, 69, 56,
    14, 17, 22, 29, 51, 87, 80, 62,
    18, 22, 37, 56, 68, 109, 103, 77,
    24, 35, 55, 64, 81, 104, 113, 92,
    49, 64, 78, 87, 103, 121, 120, 101,
    72, 92, 95, 98, 112, 100, 103, 99);

std::vector<cv::Mat_<signed char>> quantification(const std::vector<cv::Mat_<float>> &inDct)
{
    int nbBlocks = inDct.size();
    std::vector<cv::Mat_<signed char>> outQuantif;
    outQuantif.reserve(nbBlocks);

    for (int i = 0; i < nbBlocks; i++)
    {
        cv::Mat_<float> inBlock = inDct[i];
        cv::Mat_<signed char> quantif(inBlock.size());
        int rows = inBlock.rows;
        int cols = inBlock.cols;
        for(int u=0; u<rows;u++)
        {
            for(int v=0;v<cols;v++)
            {
                quantif.at<signed char>(u,v) = inBlock.at<float>(u,v) / quantifMat.at<uchar>(u,v);
            }
        }
        outQuantif.push_back(quantif);
    }
    return outQuantif;
}

std::vector<cv::Mat_<float>> inv_quantification(const std::vector<cv::Mat_<signed char>> &inQuantif)
{
    int nbBlocks = inQuantif.size();
    std::vector<cv::Mat_<float>> outBlocks;
    outBlocks.reserve(nbBlocks);

    for (int i = 0; i < nbBlocks; i++)
    {
        cv::Mat_<signed char> inQuant = inQuantif[i];
        cv::Mat_<float> block(inQuant.size());
        int rows = inQuant.rows;
        int cols = inQuant.cols;
        for (int u = 0; u < rows; u++)
        {
            for (int v = 0; v < cols; v++)
            {
                block.at<float>(u, v) = inQuant.at<signed char>(u, v) * quantifMat.at<uchar>(u, v);
            }
        }
        outBlocks.push_back(block);
    }
    return outBlocks;
}