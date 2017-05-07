#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include "opencv2/photo/photo.hpp"

int main()
{   std::string out="/home/skutukov/Pictures/segment/";
    cv::Mat src = cv::imread("/home/skutukov/Pictures/wheel/4/IMG0.png");
    if (!src.data)
    {
        return -1;
    }
    cv::fastNlMeansDenoising(src, src, 10);
//    cv::imwrite(out+"src.jpg",src);
//    // Create binary image from source image
//    cv::Mat bw;
//    cv::cvtColor(src, bw, CV_BGR2GRAY);
//    cv::threshold(bw, bw, 40, 255, CV_THRESH_BINARY);
//    cv::imwrite(out+"bw.jpg", bw);

//    // Perform the distance transform algorithm
    cv::Mat dist;
//    cv::distanceTransform(bw, dist, CV_DIST_L2, 3);

//    // Normalize the distance image for range = {0.0, 1.0}
//    // so we can visualize and threshold it
//    cv::normalize(dist, dist, 0, 1., cv::NORM_MINMAX);
//   cv::imwrite(out+"dist.jpg", dist);

//    // Threshold to obtain the peaks
//    // This will be the markers for the foreground objects
//   cv::threshold(dist, dist, .5, 1., CV_THRESH_BINARY);
//   cv::imwrite(out+"dist2.jpg", dist);

//    // Create the CV_8U version of the distance image
//    // It is needed for cv::findContours()
    cv::Canny(src, dist, 35, 110, 3);
    cv::Mat dist_8u;
    dist.convertTo(dist_8u, CV_8U);

    // Find total markers
    std::vector<std::vector<cv::Point> > contours;
    cv::findContours(dist_8u, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    int ncomp = (int) contours.size();

    // Create the marker image for the watershed algorithm
    cv::Mat markers = cv::Mat::zeros(dist.size(), CV_32SC1);

    // Draw the foreground markers
    for (int i = 0; i < ncomp; i++)
    {
        cv::drawContours(markers, contours, i, cv::Scalar::all(i+1), -1);
    }
    // Draw the background marker
    cv::circle(markers, cv::Point(5,5), 3, CV_RGB(255,255,255), -1);
    cv::imwrite(out+"contours.jpg", markers*10000);
    cv::imwrite(out+"markers.jpg", markers*10000);

    // Perform the watershed algorithm
    cv::watershed(src, markers);

    // Generate random colors
    std::vector<cv::Vec3b> colors;
    for (int i = 0; i < ncomp; i++)
    {
        int b = cv::theRNG().uniform(0, 255);
        int g = cv::theRNG().uniform(0, 255);
        int r = cv::theRNG().uniform(0, 255);

        colors.push_back(cv::Vec3b((uchar)b, (uchar)g, (uchar)r));
    }

    // Create the result image
    cv::Mat dst = cv::Mat::zeros(markers.size(), CV_8UC3);

    // Fill labeled objects with random colors
    for (int i = 0; i < markers.rows; i++)
    {
        for (int j = 0; j < markers.cols; j++)
        {
           int index = markers.at<int>(i,j);
          dst.at<cv::Vec3b>(i,j) = index > 0 && index <= ncomp ? colors[index - 1] : cv::Vec3b(0, 0, 0);
        }
    }
   cv::imwrite(out+"dst.jpg", dst);


   cv::findContours(dist_8u, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
   ncomp = (int) contours.size();

   // Create the marker image for the watershed algorithm
   markers = cv::Mat::zeros(dist.size(), CV_32SC1);

   // Draw the foreground markers
   for (int i = 0; i < ncomp; i++)
   {
       cv::drawContours(markers, contours, i, cv::Scalar::all(i+1), -1);
   }
   cv::imwrite(out+"contours_res.jpg", markers*10000);
   return 0;
}
