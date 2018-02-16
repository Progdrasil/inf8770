#include "dct.hpp"

std::vector<cv::Mat_<float>> block2dct(std::vector<cv::Mat_<uchar>> &inBlocks)
{
    int nbBlocks = inBlocks.size();
    std::vector<cv::Mat_<float>> outDct;
    outDct.reserve(nbBlocks);

    for(int i=0; i<nbBlocks;i++)
    {
        cv::Mat resultDct = dct(inBlocks[i]);
        outDct.push_back(resultDct);
    }
    return outDct;
}

cv::Mat_<float> dct(cv::Mat_<uchar> inBlock)
{
    int n = inBlock.rows;
    // Floating number matrix
    cv::Mat_<float> outDct(inBlock.size());

    for (int u = 0; u < n; u++)
    {
        for (int v = 0; v < n; v++)
        {
            float cu = 1.f / sqrt(n);
            float cv = 1.f / sqrt(n);

            if (u != 0)
                cu *= sqrt(2);

            if (v != 0)
                cv *= sqrt(2);

            for (int x = 0; x < n; x++)
            {
                for (int y = 0; y < n; y++)
                {
                    float cosXU = cos(CV_PI * (2.f * x + 1.f) * u / (2.f * n));
                    float cosYV = cos(CV_PI * (2.f * y + 1.f) * v / (2.f * n));
                    float f = outDct.at<uchar>(x, y);

                    outDct.at<float>(u, v) += cu * cv * f * cosXU * cosYV;
                }
            }
        }
    }

    return outDct;
}