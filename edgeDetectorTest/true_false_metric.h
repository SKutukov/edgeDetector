#ifndef TRUE_FALSE_METRIC_H
#define TRUE_FALSE_METRIC_H

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
class True_false_Metric
{
    True_false_Metric() = delete;
public:
    static void apply(cv::Mat test,cv::Mat src);


};

#endif // TRUE_FALSE_METRIC_H
