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
    IRR
};
/**
 * @brief procesing
 * @param image
 * @param gray_image
 * @param threh
 * @param threh_max
 */
void  procesing(cv::Mat image,cv::Mat & gray_image,int threh,int treh_max,procType type,double error=0.01);
cv::Mat grayImage(cv::Mat image);
#endif // PROCESSING_H
