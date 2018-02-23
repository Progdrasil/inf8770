#include "zigzag.hpp"

std::vector<int> blocks2vector(const std::vector<cv::Mat_<int>> &inBlocks)
{
    cv::Size sizeBlock = inBlocks[0].size();
    CV_Assert(sizeBlock.height == sizeBlock.width);
    int blockSize = sizeBlock.height;
    std::vector<int> zigzag;
    zigzag.reserve(inBlocks.size() * sizeBlock.width * sizeBlock.height);

    for(unsigned int b = 0; b < inBlocks.size(); b++)
    {
        // s is the sum of i and j coordinates, represent a diagonal
        for (int s = 0; s < blockSize * 2; s++)
        {
            int j = 0;
            if (s >= blockSize)
                j = s - blockSize + 1;

            for (; j <= s && j < blockSize; j++)
            {
                if (s % 2 == 0)
                    zigzag.push_back(inBlocks[b].at<int>(s - j, j));
                else
                    zigzag.push_back(inBlocks[b].at<int>(j, s - j));
            }
        }
    }
    return zigzag;
}

std::vector<cv::Mat_<int>> vector2blocks(const std::vector<int> &inVector)
{

}