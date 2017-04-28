#include "processing.h"
#include "irr_filter.h"
#include "denoize_filter.h"
#include "iostream"
#include "opencv2/photo/photo.hpp"

cv::Mat grayImage(cv::Mat image)
{
    std::vector<cv::Mat> channels;
    cv::Mat hsv;
    cv::cvtColor( image, hsv, CV_RGB2HSV );
    cv::split(hsv, channels);
    return channels[0];
}

void  procesing(cv::Mat image, cv::Mat & dest, int threh, int threh_max, procType type, double error)
{
    cv::fastNlMeansDenoising(image, image, 20.0);
    switch (type)
     {
     case SOBEL:
     {
        cv::Mat gray_image=grayImage(image);
        int scale = 1;
        int delta = 0;
        int ddepth = CV_16S;
        cv::threshold(image, gray_image,threh, threh_max, cv::THRESH_TOZERO_INV);
        cv::GaussianBlur( image, image, cv::Size(3,3), 0, 0, cv::BORDER_DEFAULT );
        cv::Mat grad_x, grad_y;
        cv::Mat abs_grad_x, abs_grad_y;

        /// Gradient X
        //Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
         cv::Sobel( gray_image, grad_x, ddepth, 1, 0, 3, scale, delta, cv::BORDER_DEFAULT );
         cv::convertScaleAbs( grad_x, abs_grad_x );
         /// Gradient Y
         //Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
         cv::Sobel(  gray_image, grad_y, ddepth, 0, 1, 3, scale, delta, cv::BORDER_DEFAULT );
         cv::convertScaleAbs( grad_y, abs_grad_y );
         /// Total Gradient (approximate)
         cv::addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0,  dest );
     break;
     }
     case CANNY:
     {
        cv::Canny(image,dest,threh,threh_max);
     break;
     }
     case LAPPLAS:
     {
        cv::Laplacian(image,dest,CV_64F);

     break;
     }
     case IRR:
     {
        IRR_filter irr(image,error);
        dest=irr.proc();

     break;
     }
     default:
     {
        std::cout<<" wrong procType"<<std::endl;
     }
     }

}
