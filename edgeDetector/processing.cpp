#include "auxiliary_function.h"
#include "denoize_filter.h"
#include "filter.h"
#include "filterfactory.h"
#include "iostream"
#include "irr_filter.h"
#include "opencv2/photo/photo.hpp"
#include "processing.h"

cv::Mat procesing(cv::Mat &image, const int threh, const int threh_max,
                  const filterType type, const float force, const bool isEqual)
{
    if (isEqual) {
        image = histogram_equalization(image);
    }

    if (force > 0) {
        cv::fastNlMeansDenoising(image, image, force);
    }

    return FilterFactory::createFilter(type)->proc(image, threh, threh_max);
}
