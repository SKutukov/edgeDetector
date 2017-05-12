#include "true_false_metric.h"
#include "iostream"
void True_false_Metric::apply(cv::Mat test,cv::Mat image)
{
    CV_Assert(test.size()==image.size());
    int true_positive=0,false_positive=0;
    for(int y=0;y<test.rows;y++)
    {
        const short* test_row=test.ptr<short>(y);
        const short* image_row=image.ptr<short>(y);
        for(int x=0;x<test.cols;x++)
        {
            if(test_row[x]>0)
            {
                if(test_row[x]-image_row[x]==0)
                {
                   true_positive++;
                }else
                {
                    false_positive++;
                }
            }
        }
    }
    std::cout<<true_positive<<' '<<false_positive<<std::endl;
}
