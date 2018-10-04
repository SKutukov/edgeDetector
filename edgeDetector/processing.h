#ifndef PROCESSING_H
#define PROCESSING_H

#include "filterfactory.h"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
/**
 * @brief procesing
 * @param image
 * @param gray_image
 * @param threh
 * @param threh_max
 */
cv::Mat procesing(cv::Mat &image, const int threh, const int treh_max,
                  const filterType type, const float force = 5.,
                  const bool isEq = false);
#endif // PROCESSING_H
