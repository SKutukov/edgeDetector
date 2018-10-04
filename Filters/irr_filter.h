#ifndef IRR_FILTER_H
#define IRR_FILTER_H

#include "filter.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using cv::Mat;

class IRR_filter_Imlementation
{
public:
    IRR_filter_Imlementation(Mat image, float error = float(0.01));
    Mat proc();

private:
    ConvolutionFilter conv_filter;
    float V_t = 10;
    float tresh = 35;
    float l_min = 5;
    float eps;
    float q = 1.;
    Mat edge;
    Mat image;
    Mat U_min;
    Mat L;
    Mat B;
    Mat L_h;
    Mat L_v;
    Mat zce_h;
    Mat zce_v;
    Mat control_signal;
    Mat zce;
    Mat error;
    Mat error_abs;
    Mat edge_32f;
    int max_iter_U = 5000;
    int max_iter_L = 2;
    float sigma;
    float alha(float x);
    void calculateZCE();
    void calculateControlSignal();
    void minimaze_energi_fun();
    void compute_error();
    void updateLambda();
    void updateEdge();
    Mat draw(cv::Mat src);
};

#endif // IRR_FILTER_H
