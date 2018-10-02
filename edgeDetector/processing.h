#ifndef PROCESSING_H
#define PROCESSING_H

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "filterfactory.h"
/**
 * @brief procesing
 * @param image
 * @param gray_image
 * @param threh
 * @param threh_max
 */
cv::Mat procesing(cv::Mat& image,int threh,int treh_max,filterType type,double error=1,float force=5.,bool isEq=false);
#endif // PROCESSING_H
