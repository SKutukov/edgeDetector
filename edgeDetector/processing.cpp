#include "processing.h"
#include "irr_filter.h"
#include "denoize_filter.h"
#include "iostream"
#include "opencv2/photo/photo.hpp"
#include "auxiliary_function.h"
#include "filter.h"
#include "filterfactory.h"

cv::Mat grayImage(cv::Mat image)
{
    std::vector<cv::Mat> channels;
    cv::Mat hsv;
    cv::cvtColor( image, hsv, CV_RGB2HSV );
    cv::split(hsv, channels);
    return channels[0];
}

cv::Mat  procesing(cv::Mat& image, int threh, int threh_max,
                   filterType type, double error, float force,bool isEqual)
{
    if(isEqual)
    {
        image = histogram_equalization(image);
    }

    if(force>0)
    {
        cv::fastNlMeansDenoising(image, image, force);
    }

    return FilterFactory::createFilter(type)->proc(image);
}
