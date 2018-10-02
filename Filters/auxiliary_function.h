#ifndef AUXILIARY_FUNCTION_H
#define AUXILIARY_FUNCTION_H

#include "opencv2/core/core.hpp"

double avarege_error(cv::Mat A,cv::Mat B);
cv::Mat  histogram_equalization(cv::Mat bgr_image);
#endif // AUXILIARY_FUNCTION_H

