#ifndef PROCESSING_H
#define PROCESSING_H

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
/**
 * @brief The procType enum
 */
enum procType{
    SOBEL,
    CANNY,
    LAPPLAS,
    IRR,
    KUWAHARA
};
/**
 * @brief procesing
 * @param image
 * @param gray_image
 * @param threh
 * @param threh_max
 */
cv::Mat  procesing(cv::Mat image,int threh,int treh_max,procType type,double error=0.01,double force=5.);
cv::Mat grayImage(cv::Mat image);
#endif // PROCESSING_H
