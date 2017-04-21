#include "convolution_filter.h"
Convolution_filter::Convolution_filter()
{
    kernel=Mat::ones( kernel_size, kernel_size, CV_32F )/ (float)(kernel_size*kernel_size);
}

Mat Convolution_filter::apply(Mat image)
{
    Mat dest;
    cv::filter2D(image,dest,ddepth,kernel,anchor, delta, BORDER_MODE);
    return dest;
}
