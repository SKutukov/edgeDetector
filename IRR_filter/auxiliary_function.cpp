#include "auxiliary_function.h"

double avarege_error(cv::Mat A,cv::Mat B)
{
    CV_Assert(A.rows==B.rows);
    CV_Assert(A.cols==B.cols);
    double error=0;
    for(int y=0;y<A.rows;y++)
        for(int x=0;x<A.cols;x++)
            error+=std::fabs(A.at<float>(y,x)-B.at<float>(y,x));
    error/=(A.rows)*(A.cols);
    return error;
}
