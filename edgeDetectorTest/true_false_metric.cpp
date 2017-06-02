#include "true_false_metric.h"
#include "iostream"
#include "opencv2/highgui/highgui.hpp"
True_false_Metric::True_false_Metric()
{

}

cv::Mat draw(cv::Mat& img,int thin)
{
    std::vector<std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;

    cv::findContours( img, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );
    cv::Mat drawing = cv::Mat::zeros( img.size(), CV_8UC3 );
    for( int i = 0; i< contours.size(); i++ )
       {
         cv::Scalar color = cv::Scalar( 255, 255, 255);
         cv::drawContours( drawing, contours, i, color, thin, 8, hierarchy, 0, cv::Point() );
       }

    return drawing;
}

True_false_Metric create()
{
    return True_false_Metric();
}

void True_false_Metric::apply(cv::Mat test, cv::Mat image)
{

    CV_Assert(test.size()==image.size());
    cv::threshold(test, test,200, 255, cv::THRESH_BINARY);
   // cv::threshold(image, image,200, 255, cv::THRESH_BINARY);
    int tr=0;

    cv::Mat dr=draw(test,1);
    cv::Mat dr_U8;
    cv::cvtColor( dr, dr_U8, cv::COLOR_BGR2GRAY,1);
    cv::threshold(dr_U8, dr_U8,200, 255, cv::THRESH_BINARY);
    for(int y=0; y<dr_U8.rows; y++)
    {
        const uchar* draw_row=dr_U8.ptr<uchar>(y);
        for(int x=0;x<dr_U8.cols;x++)
        {
            if(draw_row[x]>0)
            {
                tr++;
            }
        }
    }
    test=draw(dr_U8,3 );
    cv::cvtColor( test, test, cv::COLOR_BGR2GRAY,1);
    cv::threshold(test, test,200, 255, cv::THRESH_BINARY);

   // cv::imshow("test",test);
    //cv::waitKey();
    int true_positive=0,false_positive=0;
    for(int y=0; y<test.rows; y++)
    {
        const uchar* test_row=test.ptr<uchar>(y);
        const uchar* image_row=image.ptr<uchar>(y);
        for(int x=0;x<test.cols;x++)
        {
            if(image_row[x]>0)
            {
                if(test_row[x]>0)
                {
                   true_positive++;
                }else
                {
                    false_positive++;
                }
            }
        }
    }
    std::cout<<"true positive: "<<true_positive<<std::endl;
    std::cout<<"false positive: "<<false_positive<<std::endl;
    std::cout<<"true: "<<tr<<std::endl;
    std::cout<<(double)true_positive/tr<<' '<<(double)false_positive/(true_positive+false_positive)<<std::endl;
    double precision_=(double)true_positive/tr;
    double precision1=std::min(precision_,1.);
    precision+=precision1;
    double recall_=(double)false_positive/(true_positive+false_positive);
    double recall1=std::min(recall_,1.);
    recall+=recall1;
    f+=(1+b*b)*precision1*(1-recall1)/(1-recall1+b*b*precision1);
     std::cout<<"f: "<<(1+b*b)*precision1*(1-recall1)/(1-recall1+b*b*precision1)<<std::endl;
    size++;
}
