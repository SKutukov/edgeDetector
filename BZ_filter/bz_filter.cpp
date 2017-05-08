#include "bz_filter.h"


BZ_filter::BZ_filter(cv::Mat image)
{
    this->image=image.clone();
    Z=cv::Mat::ones(image.size(),CV_32FC1);
    U=image.clone();
    S=cv::Mat::ones(image.size(),CV_32FC1);
}
void BZ_filter::minimaze_S()
{

}
void BZ_filter::minimaze_Z()
{

}
void BZ_filter::minimaze_U()
{

}

cv::Mat BZ_filter::apply()
{
    double dU(2*eps),dZ(2*eps),dS(2*eps);
    cv::Mat U_n=image.clone();
    cv::Mat Z_n=cv::Mat::ones(image.size(), CV_16SC1);
    cv::Mat S_n=cv::Mat::ones(image.size(), CV_16SC1);
    while (dU>eps && dZ>eps && dS>eps )
    {
        U_n=U;
        Z_n=Z;
        S_n=S;
        minimaze_S();
        minimaze_Z();
        minimaze_U();
        dU=cv::norm(U-U_n);
        dZ=cv::norm(Z-Z_n);
        dS=cv::norm(S-S_n);
    }

    return Z;
}
