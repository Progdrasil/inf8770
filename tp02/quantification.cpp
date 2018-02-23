#include "quantification.hpp"
#include <iostream>

const cv::Mat_<uchar> basicQuantif = (cv::Mat_<uchar>(8, 8) << 16, 11, 10, 16, 24, 40, 51, 61,
                                      12, 12, 14, 19, 26, 58, 60, 55,
                                      14, 13, 16, 24, 40, 57, 69, 56,
                                      14, 17, 22, 29, 51, 87, 80, 62,
                                      18, 22, 37, 56, 68, 109, 103, 77,
                                      24, 35, 55, 64, 81, 104, 113, 92,
                                      49, 64, 78, 87, 103, 121, 120, 101,
                                      72, 92, 95, 98, 112, 100, 103, 99);

std::vector<cv::Mat_<char>> quantification(const std::vector<cv::Mat_<float>> &inDct, int level)
{
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
                if (level != 0)
                {
                    float tmp = std::nearbyint(inBlock.at<float>(u, v) / basicQuantif.at<char>(u, v) / level);
                    quantif.at<char>(u, v) = tmp;
                    if (tmp < -128 || tmp > 127)
                        std::cout << "Overflow : " << tmp << std::endl;
                }
                else if (level == 0)
                {
                    float tmp = inBlock.at<float>(u, v);
                    quantif.at<char>(u, v) = inBlock.at<float>(u, v);
                    if (tmp < -128 || tmp > 127)
                        std::cout << "Overflow : " << tmp << std::endl;
                }
            }
        }
        outQuantif.push_back(quantif);
    }
    return outQuantif;
}

std::vector<cv::Mat_<float>> inv_quantification(const std::vector<cv::Mat_<char>> &inQuantif, int level)
{
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
                if (level != 0)
                    block.at<float>(u, v) = inQuant.at<char>(u, v) * basicQuantif.at<char>(u, v) * level;
                else if (level == 0)
                    block.at<float>(u, v) = inQuant.at<char>(u, v);
            }
        }
        outBlocks.push_back(block);
    }
    return outBlocks;
}