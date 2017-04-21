#ifndef CONVOLUTION_FILTER_H
#define CONVOLUTION_FILTER_H

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using cv::Mat;

class Convolution_filter
{
public:
    Convolution_filter();
    Mat apply(Mat image);
private:
    int kernel_size=5;
    Mat kernel;
    cv::Point anchor=cv::Point(-1,-1);
    int delta=0;
    int ddepth=-1;
    int BORDER_MODE=cv::BORDER_DEFAULT;

};

#endif // CONVOLUTION_FILTER_H
