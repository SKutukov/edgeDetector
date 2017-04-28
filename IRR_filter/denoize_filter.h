#ifndef NOIZE_FILTER_H
#define NOIZE_FILTER_H

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/photo.hpp>

using cv::Mat;
class denoize_filter
{
public:
    denoize_filter(float force);
    void apply(Mat src,Mat dst);
private:
    float force;
};

#endif // NOIZE_FILTER_H
