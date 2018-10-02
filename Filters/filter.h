#ifndef FILTER_H
#define FILTER_H


class Filter
{
public:
    explicit Filter();
    virtual cv::Mat proc(const cv::Mat& source) = 0;
    virtual ~Filter() {}
};

class Sobel: public Filter
{
public:
    cv::Mat proc(const cv::Mat& source);
private:
    int scale = 1;
    int delta = 0;
    int ddepth = CV_16S;
};


class Canny: public Filter
{
public:
    cv::Mat proc(const cv::Mat& source);
private:
    int threh = 0;
    int threh_max = 256;
};

class Laplas: public Filter
{
public:
    cv::Mat proc(const cv::Mat& source);
private:
    int ddepth = CV_64F;
};

class IRR: public Filter
{
public:
    cv::Mat proc(const cv::Mat& source);
public:
    double error;
};


class Convolution_filter: public Filter
{
public:
    Convolution_filter();
    cv::Mat proc(const cv::Mat& source);
private:
    int kernel_size=5;
    Mat kernel;
    cv::Point anchor=cv::Point(-1,-1);
    int delta=0;
    int ddepth=-1;
    int BORDER_MODE=cv::BORDER_DEFAULT;

};
#endif // FILTER_H
