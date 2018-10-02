#include "auxiliary_function.h"
#include "opencv2/imgproc/imgproc.hpp"

double avarege_error(cv::Mat A, cv::Mat B)
{
    CV_Assert(A.rows==B.rows);
    CV_Assert(A.cols==B.cols);
    double error=0;
    for(int y=0;y<A.rows;y++)
    {
        for(int x=0;x<A.cols;x++)
        {
            error+=std::fabs(A.at<int>(y, x)-B.at<int>(y, x));
        }
    }
    error/=(A.rows)*(A.cols);
    return error;
}


cv::Mat  histogram_equalization(cv::Mat bgr_image)
{
       cv::Mat lab_image;
       cv::cvtColor(bgr_image, lab_image, CV_BGR2Lab);

       // Extract the L channel
       std::vector<cv::Mat> lab_planes(3);
       cv::split(lab_image, lab_planes);  // now we have the L image in lab_planes[0]

       // apply the CLAHE algorithm to the L channel
       cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
       clahe->setClipLimit(1);
       clahe->setTilesGridSize(cv::Size(8,8));
       cv::Mat dst;
       clahe->apply(lab_planes[0], dst);

       // Merge the the color planes back into an Lab image
       dst.copyTo(lab_planes[0]);
       cv::merge(lab_planes, lab_image);

      // convert back to RGB
      cv::Mat image_clahe;
      cv::cvtColor(lab_image, image_clahe, CV_Lab2BGR);

      // display the results  (you might also want to see lab_planes[0] before and after).
      return image_clahe;
 }
