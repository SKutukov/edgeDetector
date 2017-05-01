#include "gtest/gtest.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgcodecs/imgcodecs.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

TEST(Test_sample, test_sample)
{

    cv::Mat image = cv::imread("/home/skutukov/Pictures/edges/1.jpg");
    cv::Mat test_image = cv::imread("/home/skutukov/Pictures/test/1.jpg");
    cv::Mat dU=image-test_image;
    //cv::imshow("test",image-test_image);
    //cv::waitKey(0);
   // test_image=cv::Mat::zeros( test_image.size(), CV_8UC3 );
    int a=0,b=0;
    for(int y=0;y<image.rows;y++)
    {
        const cv::Point3_<short>* test_image_row=test_image.ptr<cv::Point3_<short>>(y);
        const cv::Point3_<short>* dU_row=dU.ptr<cv::Point3_<short>>(y);
        for(int x=0;x<image.cols;x++)
        {
           if(test_image_row[x].x>0 || test_image_row[x].y>0 || test_image_row[x].z>0)
           {
                b++;
           }
           if(dU_row[x].x>0 || dU_row[x].y>0 || dU_row[x].z>0)
           {
                a++;
           }

        }
    }
    std::cout<<a<<' '<<b<<std::endl;
 //   ASSERT_NEAR(1,1,1e-4);

    SUCCEED();
}

