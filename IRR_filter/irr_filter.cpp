#include "irr_filter.h"

#include <iostream>
IRR_filter::IRR_filter(Mat image):
    conv_filter()
{
    CV_Assert(image.type()==CV_8UC1);
    B=Mat::zeros(image.rows,image.cols,CV_8UC1);
    U_min=image.clone();
    this->image=image.clone();
    L_h=Mat::zeros(image.rows,image.cols,CV_32FC1);
    L_v=Mat::zeros(image.rows,image.cols,CV_32FC1);
    L=Mat::zeros(image.rows,image.cols,CV_32FC1);
    for(int y=1;y<image.rows-1;y++)
    {
        float* L_h_row = L_h.ptr<float>(y);
        float* L_v_row = L_v.ptr<float>(y);
        float* L_row = L.ptr<float>(y);

        const short* image_row1=image.ptr<short>(y-1);
        const short* image_row2=image.ptr<short>(y);
        const short* image_row3=image.ptr<short>(y+1);
        for(int x=1;x<image.cols-1;x++)
        {
                L_v_row[x]=std::max(fabs(image_row1[x]-image_row2[x]),fabs(image_row3[x]-image_row2[x]));
                L_h_row[x]=std::max(fabs(image_row2[x]-image_row2[x-1]),fabs(image_row2[x]-image_row2[x+1]));
                L_row[x]=std::max(L_h_row[x],L_v_row[x]);
        }
    }

    for(auto i=0;i<B.rows;i++)
        for (auto j=0;j<B.rows;j++)
                {
                if(image.at<int>(i,j))
                {
                    B.at<int>(i,j)=1;
                }
        }


}

Mat IRR_filter::proc()
{
    edge=Mat::zeros(image.rows,image.cols,CV_8U);
    double dL=2*eps;
    while(dL>eps)
    {
        ///2)-----------------mimimaze energi functional -------------------------
        minimaze_energi_fun();
        ///3.a.b)-----------------compute error  -------------------------
        compute_error();
        ///3.c)-----------------compute control signal  -------------------------
        calculateControlSignal();
        ///3.d)-----------------compute magnutude of zero crossing in error signal zce(x,y)  -------------------------
        calculateZCE();
        ///4)------------------ update edges ------------------------------------------
        updateEdge();
        ///5)------------------ update lambda -----------------------------------------
        updateLambda();
        dL=eps/2;
    }

    return edge;
}
#include <iostream>
void IRR_filter::minimaze_energi_fun()
{
    Mat U=Mat::zeros(image.rows,image.cols,CV_32FC1);
    Mat U_n;
    image.convertTo(U_n,CV_32FC1);
    int begin=1;
    double dU=2*eps;
    int i=0;
    while(dU>eps && i<10)
    {
        for(int y=begin;y<U.rows-1;y++)
        {
            const short* B_row = B.ptr<short>(y);
            const short* U_n_row = U_n.ptr<short>(y);
            const float* L_h_row_y = L_h.ptr<float>(y);
            const float* L_h_row_y_1 = L_h.ptr<float>(y+1);
            const float* L_v_row = L_v.ptr<float>(y);
            for (int x=begin;x<U.cols-1;x++)
                {
                    float T=B_row[x]+L_h_row_y[x]+L_h_row_y_1[x]+L_v_row[x]+L_v_row[x+1];
                    int a=(L_h_row_y[x]*U.at<float>(y-1,x)+L_h_row_y_1[x]*U_n.at<float>(y+1,x)+
                          L_v_row[x]*U.at<float>(y,x-1)+L_v_row[x+1]*U.at<float>(y,x+1));
                    U.at<short>(x,y)=U_n_row[x]-q*(T*U_n_row[x]-B_row[x]*image.at<short>(y,x)-a)/T;
                }
        }
        dU=cv::norm(U-U_n);
        std::cout<<dU<<std::endl;
        U_n=U.clone();
        i++;
    }
    U_min=U.clone();
    std::cout<<i<<std::endl;
}
void IRR_filter::compute_error()
{
    error=Mat(image.rows,image.cols,CV_32F);
    error_abs=Mat(image.rows,image.cols,CV_32F);
    sigma=0;
    for(int y=0;y<image.rows;y++)
    {
        auto error_row = error.ptr<float>(y);
        auto error_abs_row =error_abs.ptr<float>(y);
        const short* image_row = image.ptr<short>(y);
        const float* U_min_row = U_min.ptr<float>(y);
        for(int x=0;x<image.cols;x++)
        {
            double error=U_min_row[x]-image_row[x];
            ///todo: why nan?
            if(std::isnan(error))
                error=0;
            error_abs_row[x]=std::fabs(error);
            error_row[x]=error;
            sigma+=error;
        }
    }
    sigma/=(image.rows+1)*(image.cols+1);
    std::cout<<"error abs"<<std::endl;
    std::cout<<cv::norm(error_abs)<<std::endl;
    std::cout<<"error"<<std::endl;
    std::cout<<cv::norm(error_abs)<<std::endl;

}

void IRR_filter::calculateControlSignal()
{
    //std::cout<<cv::norm(error_abs,cv::NORM_L2)<<std::endl;
    cv::Mat A=conv_filter.apply(error_abs);
    for(auto i=0;i<A.rows;i++)
        for (auto j=0;j<A.cols;j++)
                    {
            //std::cout<<A.at<float>(i,j)<<std::endl;
            A.at<float>(i,j)-=sigma;
        }
    control_signal=conv_filter.apply(A);
   //std::cout<<cv::norm(control_signal,cv::NORM_L2)<<std::endl;
}
void IRR_filter::calculateZCE()
{
    zce=Mat(image.rows,image.cols,CV_32F);
    zce.setTo(cv::Scalar(0.));
    for(int y=1;y<image.rows-1;y++)
    {
        float* zce_row = zce.ptr<float>(y);
        float* error_row1 = error.ptr<float>(y-1);
        float* error_row2 = error.ptr<float>(y);
        float* error_row3 = error.ptr<float>(y+1);
        for(int x=1;x<image.cols-1;x++)
        {
            float zce_h=0.;
            float zce_v=0.;
            if((error_row2[x-1]*error_row2[x+1])<0)
            {
                 zce_h=std::fabs(error_row2[x-1]-error_row2[x+1]);
            }
            if((error_row2[x-1]*error_row2[x+1])<0)
            {
                 zce_v=std::fabs(error_row1[x]-error_row3[x]);
            }

            zce_row[x]=std::max<float>(zce_h,zce_v);

        }
    }
}
void IRR_filter::updateEdge()
{
    for(int y=0;y<image.rows;y++)
    {
        short* edge_row = edge.ptr<short>(y);
        const float* zce_row = zce.ptr<float>(y);
        const float* control_signal_row = control_signal.ptr<float>(y);
        for(int x=0;x<image.cols;x++)
        {
            //&& control_signal_row[x]>0
            if(edge_row[x]==0  && zce_row[x]>0)
            {
                edge_row[x]=static_cast<short>(zce_row[x]);
            }

        }
    }
}
void IRR_filter::updateLambda()
{

}
