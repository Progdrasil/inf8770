#include "zigzag.hpp"

std::vector<char> blocks2vector(const std::vector<cv::Mat_<char>> &inBlocks)
{
    cv::Size sizeBlock = inBlocks[0].size();
    CV_Assert(sizeBlock.height == sizeBlock.width);
    int blockSize = sizeBlock.height;
    std::vector<char> zigzag;
    zigzag.reserve(inBlocks.size() * sizeBlock.width * sizeBlock.height);

    for (uint b = 0; b < inBlocks.size(); b++)
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
                    zigzag.push_back(inBlocks[b].at<char>(s - j, j));
                else
                    zigzag.push_back(inBlocks[b].at<char>(j, s - j));
            }
        }
    }
    return zigzag;
}

std::vector<cv::Mat_<char>> vector2blocks(const std::vector<char> &inVector, cv::Size &sizeBlock, uint nbBlocks)
{
    std::vector<cv::Mat_<char>> outBlocks;
    outBlocks.reserve(nbBlocks);

    for (uint b = 0; b < nbBlocks; b++)
    {
        cv::Mat_<char> block(sizeBlock);
        int blockSize = sizeBlock.height;

        int index = 0;
        for (int s = 0; s < blockSize * 2; s++)
        {
            int j = 0;
            if (s >= blockSize)
                j = s - blockSize + 1;

            for (; j <= s && j < blockSize; ++j)
            {
                int indexVector = b * blockSize * blockSize + index++;
                if (s & 1)
                {
                    block.at<char>(j, s - j) = inVector[indexVector];
                }
                else
                {
                    block.at<char>(s - j, j) = inVector[indexVector];
                }
            }
        }
        outBlocks.push_back(block);
    }
    return outBlocks;
}