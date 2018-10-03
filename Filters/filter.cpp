#include "filter.h"
#include "opencv2/photo/photo.hpp"
#include "irr_filter.h"
#include "auxiliary_function.h"


cv::Mat grayImage(const cv::Mat& image)
{
    std::vector<cv::Mat> channels;
    cv::Mat hsv;
    cv::cvtColor( image, hsv, CV_RGB2HSV );
    cv::split(hsv, channels);
    return channels[0];
}

cv::Mat Sobel::proc(const cv::Mat &source, int threh, int threh_max)
{
    cv::Mat dest;
    cv::Mat gray_image=grayImage(source);
    cv::threshold(source, gray_image, threh, threh_max, cv::THRESH_TOZERO_INV);
    cv::GaussianBlur( source, source, cv::Size(3,3), 0, 0, cv::BORDER_DEFAULT );
    cv::Mat grad_x, grad_y;
    cv::Mat abs_grad_x, abs_grad_y;

    /// Gradient X
     cv::Sobel( gray_image, grad_x, ddepth, 1, 0, 3, scale, delta, cv::BORDER_DEFAULT );
     cv::convertScaleAbs( grad_x, abs_grad_x );
     /// Gradient Y
     cv::Sobel(  gray_image, grad_y, ddepth, 0, 1, 3, scale, delta, cv::BORDER_DEFAULT );
     cv::convertScaleAbs( grad_y, abs_grad_y );
     /// Total Gradient (approximate)
     cv::addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0,  dest );
     return  dest;
}

cv::Mat Canny::proc(const cv::Mat &source,int threh, int threh_max)
{
    cv::Mat dest;
    cv::Canny(source,dest,threh,threh_max);
    return  dest;
}

cv::Mat Laplas::proc(const cv::Mat &source,int threh, int threh_max)
{
    cv::Mat dest;
    cv::Laplacian(source,dest,CV_64F);
    return dest;
}

cv::Mat IRR::proc(const cv::Mat &source,int threh, int threh_max)
{
    IRR_filter_Imlementation irr(source,error);
    return irr.proc();
}

ConvolutionFilter::ConvolutionFilter()
{
    kernel=Mat( kernel_size, kernel_size, CV_32F );
    kernel.setTo(1./kernel_size*kernel_size);
}

Mat ConvolutionFilter::proc(const cv::Mat &source,int threh, int threh_max)
{
    Mat dest;
    cv::filter2D(source, dest, ddepth, kernel, anchor, delta, BORDER_MODE);
    return dest;
}

//DenoizeFilter::DenoizeFilter(float force)
//{
//    this->force = force;
//}

//cv::Mat DenoizeFilter::proc(const cv::Mat &source)
//{
//        Mat U=source.clone();
//        Mat U_n=source.clone();
//        double dU(2*eps);
//        int i=0;
//        while(dU>eps && i<20000)
//        {
//            double dx,dy,D;
//            for(int y=1;y<U.rows-1;y++)
//            {
//                const short* U_n_row1 = U_n.ptr<short>(y-1);
//                const short* U_n_row2 = U_n.ptr<short>(y);
//                const short* U_n_row3 = U_n.ptr<short>(y+1);
//                    for(int x=1;x<U.cols-1;y++)
//                    {
//                        dx=(U_n_row2[x+1]-U_n_row2[x-1])/2;
//                        dy=(U_n_row3[x]-U_n_row1[x])/2;
//                        D=std::sqrt(dx*dx+dy*dy);
//                        //i forget what it should do
//                        U.at<short>(y,x)=1-tay*(1)+tay*(1);
//                    }
//            }
//            dU=avarege_error(U,U_n);
//            U_n=U.clone();

//            i++;
//        }
//        return U.clone();
//}
