#include "irr_filter.h"
#include "auxiliary_function.h"
#include <iostream>
IRR_filter_Imlementation::IRR_filter_Imlementation(Mat image, double error):
    conv_filter(),eps(error)
{
    B=Mat::ones(image.size(), CV_8UC1);
    U_min=cv::Mat::zeros(image.size(), CV_32FC1);
    ///-----------------------------------------------
    this->image=image.clone();

    cv::cvtColor( this->image, this->image, cv::COLOR_BGR2GRAY,1);
    ///------------------------------------------------
    L_h=Mat::ones(image.size(),CV_32FC1);
    L_v=Mat::ones(image.size(),CV_32FC1);
    L=Mat::ones(image.size(),CV_32FC1);
    L_h.setTo(cv::Scalar(100.));
    L_v.setTo(cv::Scalar(100.));

}
#include "iostream"
Mat trac(cv::Mat src)
{
    src.convertTo(src,CV_8UC1);
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours( src, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

    /// Draw contours
    Mat drawing = Mat::zeros( src.size(), CV_8UC3 );
    for( size_t i = 0; i< contours.size(); i++ )
    {
         cv::Scalar color = cv::Scalar(255, 255, 255);
         drawContours( drawing, contours, i, color, 1, 8, hierarchy, 0, cv::Point() );
    }
    return src;
}

Mat IRR_filter_Imlementation::proc()
{
    edge=Mat::zeros(image.size(), CV_16SC1);
    float dE=2*eps;
    int i=0;
    while(dE>eps && i<max_iter_L)
    {
        ///2)-----------------mimimaze energi functional -------------------------
        minimaze_energi_fun();
        ///3.a.b)-----------------compute error  -------------------------
        compute_error();
        std::cout<<" sigma: "<< sigma<<std::endl;
        tresh=float(0.1)*sigma;
        ///3.c)-----------------compute control signal  -------------------------
        calculateControlSignal();
        ///3.d)-----------------compute magnutude of zero crossing in error signal zce(x,y)  -------------------------
        calculateZCE();
        ///4)------------------ update edges ------------------------------------------
        Mat E_old=edge.clone();
        updateEdge();
        ///5)------------------ update lambda -----------------------------------------
        updateLambda();
        dE=static_cast<float>(cv::norm(edge-E_old));
        std::cout<<i<<"|dE: "<<dE<<std::endl;
        ++i;
    }
    return zce;
}

void IRR_filter_Imlementation::minimaze_energi_fun()
{
    Mat U_n=Mat::zeros(image.size(), CV_32FC1);
    int begin=1;
    float dU(2*eps);

    int i=0;
    while(dU>eps && i<max_iter_U)
    {
        for(int y=begin;y<U_min.rows-1;y++)
        {
            const unsigned char* B_row = B.ptr<unsigned char>(y);
            const unsigned char* image_row = image.ptr<unsigned char>(y);
            const float* U_n_row = U_n.ptr<float>(y);
            const float* L_h_row_y = L_h.ptr<float>(y);
            const float* L_h_row_y_1 = L_h.ptr<float>(y+1);
            const float* L_v_row = L_v.ptr<float>(y);
            const float* U_min_row = U_min.ptr<float>(y-1);
            float* U_min_row1 = U_min.ptr<float>(y);
            for (int x=begin;x<U_min.cols-1;x++)
                {
                    float T=B_row[x]+L_h_row_y[x]+L_h_row_y_1[x]+L_v_row[x]+L_v_row[x+1];
                    float a=(L_h_row_y[x]*U_min_row[x]+L_h_row_y_1[x]*U_n.at<float>(y+1,x)+
                          L_v_row[x]*U_min_row1[x-1]+L_v_row[x+1]*U_min_row1[x+1]);
                    U_min_row1[x]=U_n_row[x]-q*(T*U_n_row[x]-B_row[x]*image_row[x]-a)/T;
                }
        }
        dU=static_cast<float>(cv::norm(U_min-U_n));
        U_n=U_min.clone();

        i++;
    }
    std::cout<<i<<"|dU:"<<dU<<std::endl;
    std::cout<<"energi:"<<cv::norm(U_min)<<std::endl;
}
void IRR_filter_Imlementation::compute_error()
{
    Mat image_32f;
    image.convertTo(image_32f,CV_32FC1);
    error=U_min-image_32f;
    error_abs=abs(error);
    sigma=0;
    for(int y=0; y<image.rows; y++)
    {
        //const float* error_row = error.ptr<float>(y);
        float* error_abs_row =error_abs.ptr<float>(y);

        for(int x=0;x<image.cols;x++)
        {
            float error = error_abs_row[x];
            sigma+=error*error;
        }
    }
    std::cout<<"sum: "<<sigma<<std::endl;
    sigma/=(image.rows+1)*(image.cols+1);
    sigma=std::sqrt(sigma);

}

void IRR_filter_Imlementation::calculateControlSignal()
{
   cv::GaussianBlur( error_abs, control_signal, cv::Size(3,3), 0, 0, cv::BORDER_DEFAULT );
   cv::subtract(control_signal, cv::Scalar_<float>(sigma), control_signal);
   cv::GaussianBlur( control_signal, control_signal, cv::Size(25,25), 0, 0, cv::BORDER_DEFAULT );
}
void IRR_filter_Imlementation::calculateZCE()
{
    zce=Mat::zeros(image.size(), CV_32F);
    zce_h=Mat::zeros(image.size(), CV_32F);
    zce_v=Mat::zeros(image.size(), CV_32F);
    zce.setTo(cv::Scalar(0.));
    for(int y=1;y<image.rows-1;y++)
    {

        float* zce_row = zce.ptr<float>(y);
        float* error_row1 = error.ptr<float>(y-1);
        float* error_row2 = error.ptr<float>(y);
        float* error_row3 = error.ptr<float>(y+1);
        float* zce_row_h=zce_h.ptr<float>(y);
        float* zce_row_v=zce_v.ptr<float>(y);
        for(int x=1; x<image.cols-1; x++)
        {
            float shr=(error_row1[x+1]+error_row2[x+1]+error_row3[x+1])/3;
            float shl=(error_row1[x-1]+error_row2[x-1]+error_row3[x-1])/3;
            float sht=(error_row3[x+1]+error_row3[x]+error_row3[x-1])/3;
            float shb=(error_row1[x+1]+error_row1[x]+error_row1[x-1])/3;
            float zce_h=0.;
            float zce_v=0.;
            if((shr*shl)<0)
            {
                 zce_h=std::max(std::fabs(shr),std::fabs(shl));
                 zce_row_h[x]=zce_h;
            }
            if((sht*shb)<0)
            {
                 zce_v=std::max(std::fabs(sht),std::fabs(shb));
                 zce_row_v[x]=zce_v;
            }
            zce_row[x]=std::max<float>(zce_h,zce_v);
        }
    }
    std::cout<<"zce:"<<cv::norm(zce)<<std::endl;
}
void IRR_filter_Imlementation::updateEdge()
{
     Mat control_signal_u8c1;
     control_signal.convertTo(control_signal_u8c1,CV_8UC1);
     edge_32f=Mat::zeros(image.size(), CV_32FC1);
     cv::add(zce,cv::Scalar(0),edge_32f,control_signal_u8c1);

    for(int y=0; y<image.rows; y++)
    {
        float* edge_row = edge_32f.ptr<float>(y);
        const float* zce_row = zce.ptr<float>(y);
        const float* control_signal_row = control_signal.ptr<float>(y);
        for(int x=0; x<image.cols; x++)
        {
            if( control_signal_row[x]>0)
            {
                edge_row[x]=zce_row[x];
            }

        }
    }
    edge_32f.convertTo(edge,CV_16SC1);
}
float IRR_filter_Imlementation::alha(float x)
{
    float exp=std::exp(-x/V_t);
    return l_min*(1-exp)+x*(exp);
}

void IRR_filter_Imlementation::updateLambda()
{
    for(int y=0;y<L.rows;y++)
    {
        const float* control_signal_row = control_signal.ptr<float>(y);
        const float* zce_row = zce.ptr<float>(y);
        float* L_h_row = L_h.ptr<float>(y);
        float* L_v_row = L_v.ptr<float>(y);
        float* L_row = L.ptr<float>(y);
        for(int x=0;x<L.cols;x++)
        {
            if(control_signal_row[x]>0 && zce_row[x]>tresh && L_row[x]>l_min )
            {
                L_v_row[x]=alha(L_v_row[x]);
                L_h_row[x]=alha(L_h_row[x]);
                L_row[x]=alha(L_row[x]);
            }
        }
    }
}
