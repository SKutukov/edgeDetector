#ifndef TRUE_FALSE_METRIC_H
#define TRUE_FALSE_METRIC_H

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
class True_false_Metric
{
public:
    True_false_Metric();
    void apply(cv::Mat test, cv::Mat src);
    double precision=0;
    double recall=0;
    double size=0;

};

#endif // TRUE_FALSE_METRIC_H
