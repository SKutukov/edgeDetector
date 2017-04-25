#ifndef IRR_FILTER_H
#define IRR_FILTER_H


#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "convolution_filter.h"
using cv::Mat;

class IRR_filter
{
public:
    IRR_filter(Mat image,double error=0.01);
    Mat proc();
private:
    Convolution_filter conv_filter;
    double V_t=10;
    double tresh=35;
    double l_min=5;
    double eps;
    double q=1.;
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
    double alha(float x);
    void calculateZCE();
    void calculateControlSignal();
    void minimaze_energi_fun();
    void compute_error();
    void updateLambda();
    void updateEdge();
};

#endif // IRR_FILTER_H
