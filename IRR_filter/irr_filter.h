#ifndef IRR_FILTER_H
#define IRR_FILTER_H


#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "convolution_filter.h"
using cv::Mat;

class IRR_filter
{
public:
    IRR_filter(Mat image);
    Mat proc();
private:
    Convolution_filter conv_filter;
    double eps=0.001;
    ///
    Mat edge;
    Mat image;
    Mat U_min;
    Mat L;
    Mat B;
    Mat L_h;
    Mat L_v;
    Mat control_signal;
    Mat zce;
    Mat error;
    Mat error_abs;
    double sigma;
    double q;
    void calculateZCE();
    void calculateControlSignal();
    void minimaze_energi_fun();
    void compute_error();
    void updateLambda();
    void updateEdge();
};

#endif // IRR_FILTER_H
