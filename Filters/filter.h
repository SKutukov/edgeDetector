#ifndef FILTER_H
#define FILTER_H

#include <opencv2/core/core.hpp>
class Filter
{
public:
    explicit Filter(){}
    virtual cv::Mat proc(const cv::Mat& source,
                         int threh=0, int threh_max=256) = 0;
    virtual ~Filter() {}
};

class Sobel: public Filter
{
public:
    cv::Mat proc(const cv::Mat& source,
                 int threh=0, int threh_max=256);
private:
    int scale = 1;
    int delta = 0;
    int ddepth = CV_16S;
};


class Canny: public Filter
{
public:
    cv::Mat proc(const cv::Mat& source,
                 int threh=0, int threh_max=256);
};

class Laplas: public Filter
{
public:
    cv::Mat proc(const cv::Mat& source,
                 int threh=0, int threh_max=256);
private:
    int ddepth = CV_64F;
};

class IRR: public Filter
{
public:
    cv::Mat proc(const cv::Mat& source,
                 int threh=0, int threh_max=256);
public:
    double error;
};


class ConvolutionFilter: public Filter
{
public:
    ConvolutionFilter();
    cv::Mat proc(const cv::Mat& source,
                 int threh=0, int threh_max=256);
private:
    int kernel_size=5;
    cv::Mat kernel;
    cv::Point anchor=cv::Point(-1,-1);
    int delta=0;
    int ddepth=-1;
    int BORDER_MODE=cv::BORDER_DEFAULT;

};

//class DenoizeFilter: public Filter
//{
//public:
//    DenoizeFilter(float force);
//    cv::Mat proc(const cv::Mat& source);
//private:
//    double eps=0.1;
//    float force;
//    double tay=0.25;
//};
#endif // FILTER_H
