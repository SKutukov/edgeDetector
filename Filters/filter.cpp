#include "filter.h"
#include "opencv2/photo/photo.hpp"
#include "irr_filter.h"

cv::Mat grayImage(const cv::Mat& image)
{
    std::vector<cv::Mat> channels;
    cv::Mat hsv;
    cv::cvtColor( image, hsv, CV_RGB2HSV );
    cv::split(hsv, channels);
    return channels[0];
}

cv::Mat Sobel::proc(const cv::Mat &source)
{
    cv::Mat dest;
    cv::Mat gray_image=grayImage(source);
    cv::threshold(source, gray_image, threh, threh_max, cv::THRESH_TOZERO_INV);
    cv::GaussianBlur( source, source, cv::Size(3,3), 0, 0, cv::BORDER_DEFAULT );
    cv::Mat grad_x, grad_y;
    cv::Mat abs_grad_x, abs_grad_y;

    /// Gradient X
     cv::Sobel( gray_image, grad_x, ddepth, 1, 0, 3, scale, delta, cv::BORDER_DEFAULT );
     cv::convertScaleAbs( grad_x, abs_grad_x );
     /// Gradient Y
     cv::Sobel(  gray_image, grad_y, ddepth, 0, 1, 3, scale, delta, cv::BORDER_DEFAULT );
     cv::convertScaleAbs( grad_y, abs_grad_y );
     /// Total Gradient (approximate)
     cv::addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0,  dest );
     return  dest;
}

cv::Mat Canny::proc(const cv::Mat &source)
{
    cv::Mat dest;
    cv::Canny(source,dest,threh,threh_max);
    return  dest;
}

cv::Mat Laplas::proc(const cv::Mat &source)
{
    cv::Mat dest;
    cv::Laplacian(source,dest,CV_64F);
    return dest;
}

cv::Mat IRR::proc(const cv::Mat &source)
{
    IRR_filter irr(source,error);
    return irr.proc();
}

Convolution_filter::Convolution_filter()
{
    kernel=Mat( kernel_size, kernel_size, CV_32F );
    kernel.setTo(1./kernel_size*kernel_size);
}

Mat Convolution_filter::proc(const cv::Mat &source)
{
    Mat dest;
    cv::filter2D(source, dest, ddepth, kernel, anchor, delta, BORDER_MODE);
    return dest;
}
