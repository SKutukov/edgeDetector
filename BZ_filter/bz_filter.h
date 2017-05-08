#ifndef BZ_FILTER_H
#define BZ_FILTER_H

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

class  BZ_filter
{
public:
    BZ_filter(cv::Mat image);
    cv::Mat apply();
    cv::Mat image;
private:
    cv::Mat Z,S,U;
    void minimaze_Z();
    void minimaze_U();
    void minimaze_S();
    double eps=0.001;
    double a=0;
    double b=0;
    double m=0;
};

#endif // BZ_FILTER_H
