#include "irr_filter.h"
#include "auxiliary_function.h"
#include <iostream>
IRR_filter::IRR_filter(Mat image, double error):
    conv_filter(),eps(error)
{
    //CV_Assert(image.type()==CV_8UC1);
    B=Mat::ones(image.size(), CV_8UC1);
    U_min=cv::Mat::zeros(image.size(), CV_32FC1);
    ///-----------------------------------------------
    this->image=image.clone();
    //this->image.convertTo(this->image,CV_8UC3);

    cv::cvtColor( this->image, this->image, cv::COLOR_BGR2GRAY,1);
    //std::cout<<this->image.channels()<<std::endl;
    //this->image.convertTo(this->image,CV_16SC1);
    ///------------------------------------------------
    L_h=Mat::ones(image.size(),CV_32FC1);
    L_v=Mat::ones(image.size(),CV_32FC1);
    L=Mat::ones(image.size(),CV_32FC1);
    L_h.setTo(cv::Scalar(100.));
    L_v.setTo(cv::Scalar(100.));


//    int scale = 1;
//    int delta = 0;
//    int ddepth = CV_32FC1;

//    cv::Sobel( this->image, L_h, ddepth, 1, 0, 3, scale, delta, cv::BORDER_DEFAULT );
//    cv::convertScaleAbs( L_h, L_h );
//    /// Gradient Y
//    cv::Sobel(  this->image, L_v, ddepth, 0, 1, 3, scale, delta, cv::BORDER_DEFAULT );
//    cv::convertScaleAbs( L_v, L_v );

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
                L_v_row[x]=std::max(fabs(image_row1[x]-image_row2[x]), fabs(image_row3[x]-image_row2[x]));
                L_h_row[x]=std::max(fabs(image_row2[x]-image_row2[x-1]), fabs(image_row2[x]-image_row2[x+1]));
                L_row[x]=std::max(L_h_row[x], L_v_row[x]);
        }
    }

}

Mat IRR_filter::draw(cv::Mat src)
{
    Mat dst(src.size(),CV_16SC1);
    int aperture_size = 3;
    const int cn = 1;

    int low=2500;
    int high=3000;
    ptrdiff_t mapstep = src.cols + 2;
    cv::AutoBuffer<float> buffer((src.cols+2)*(src.rows+2) + cn * mapstep * 3 * sizeof(int));

    float* mag_buf[3];
    mag_buf[0] = (float*)(float*)buffer;
    mag_buf[1] = mag_buf[0] + mapstep*cn;
    mag_buf[2] = mag_buf[1] + mapstep*cn;
    memset(mag_buf[0], 0, /* cn* */mapstep*sizeof(int));

    int* map = (int*)(mag_buf[2] + mapstep*cn);
    memset(map, 1, mapstep);
    memset(map + mapstep*(src.rows + 1), 1, mapstep);

    int maxsize = std::max(1 << 10, src.cols * src.rows / 10);
    std::vector<int*> stack(maxsize);
    int **stack_top = &stack[0];
    int **stack_bottom = &stack[0];

    /* sector numbers
       (Top-Left Origin)

        1   2   3
         *  *  *
          * * *
        0*******0
          * * *
         *  *  *
        3   2   1
    */

    #define CANNY_PUSH(d)    *(d) = uchar(2), *stack_top++ = (d)
    #define CANNY_POP(d)     (d) = *--stack_top

    // calculate magnitude and angle of gradient, perform non-maxima suppression.
    // fill the map with one of the following values:
    //   0 - the pixel might belong to an edge
    //   1 - the pixel can not belong to an edge
    //   2 - the pixel does belong to an edge
    for (int i = 0; i <= src.rows; i++)
    {
        float* _norm = mag_buf[(i > 0) + 1] + 1;
        float* control_signal_row= control_signal.ptr<float>(i);
        if (i < src.rows)
        {
            for (int j = 0; j < src.cols*cn; j++)
            {
                _norm[j] = control_signal_row[j];
            }

            _norm[-1] = _norm[src.cols] = 0;
        }
        else
            memset(_norm-1, 0, /* cn* */mapstep*sizeof(int));

        // at the very beginning we do not have a complete ring
        // buffer of 3 magnitude rows for non-maxima suppression
        if (i == 0)
            continue;

        int* _map = map + mapstep*i + 1;
        _map[-1] = _map[src.cols] = 1;

        float* _mag = mag_buf[1] + 1; // take the central row

        if ((stack_top - stack_bottom) + src.cols > maxsize)
        {
            int sz = (int)(stack_top - stack_bottom);
            maxsize = maxsize * 3/2;
            stack.resize(maxsize);
            stack_bottom = &stack[0];
            stack_top = stack_bottom + sz;
        }

        int prev_flag = 0;
        for (int j = 0; j < src.cols; j++)
        {
            int m = _mag[j];

            if (m > low)
            {
                goto __ocv_canny_push;

            }
            prev_flag = 0;
            _map[j] = int(1);
            continue;
__ocv_canny_push:
            if (!prev_flag && m > high && _map[j-mapstep] != 2)
            {
                CANNY_PUSH(_map + j);
                prev_flag = 1;
            }
            else
                _map[j] = 0;
        }

        // scroll the ring buffer
        _mag = mag_buf[0];
        mag_buf[0] = mag_buf[1];
        mag_buf[1] = mag_buf[2];
        mag_buf[2] = _mag;
    }

    // now track the edges (hysteresis thresholding)
    while (stack_top > stack_bottom)
    {
        int* m;
        if ((stack_top - stack_bottom) + 8 > maxsize)
        {
            int sz = (int)(stack_top - stack_bottom);
            maxsize = maxsize * 3/2;
            stack.resize(maxsize);
            stack_bottom = &stack[0];
            stack_top = stack_bottom + sz;
        }

        CANNY_POP(m);

        if (!m[-1])         CANNY_PUSH(m - 1);
        if (!m[1])          CANNY_PUSH(m + 1);
        if (!m[-mapstep-1]) CANNY_PUSH(m - mapstep - 1);
        if (!m[-mapstep])   CANNY_PUSH(m - mapstep);
        if (!m[-mapstep+1]) CANNY_PUSH(m - mapstep + 1);
        if (!m[mapstep-1])  CANNY_PUSH(m + mapstep - 1);
        if (!m[mapstep])    CANNY_PUSH(m + mapstep);
        if (!m[mapstep+1])  CANNY_PUSH(m + mapstep + 1);
    }

    // the final pass, form the final image
    const int* pmap = map + mapstep + 1;

    for (int i = 0; i < src.rows; i++, pmap += mapstep)
    {
        int* pdst = dst.ptr<int>(i);
        for (int j = 0; j < src.cols; j++)
        {
            pdst[j] = (int)-(pmap[j] >> 1);
        }
    }
    return dst;
}

#include "iostream"
Mat trac(cv::Mat src)
{
    src.convertTo(src,CV_8UC1);
//    std::cout<<src.type()<<std::endl;
//    std::vector<std::vector<cv::Point> > contours;
//    std::vector<cv::Vec4i> hierarchy;
//    cv::findContours( src, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

//    /// Draw contours
//    Mat drawing = Mat::zeros( src.size(), CV_8UC3 );
//    for( int i = 0; i< contours.size(); i++ )
//       {
//         cv::Scalar color = cv::Scalar(255, 255, 255);
//         drawContours( drawing, contours, i, color, 1, 8, hierarchy, 0, cv::Point() );
//       }
    cv::Canny(src,src,0,1000);
    return src;
}

#include "opencv2/highgui/highgui.hpp"
Mat IRR_filter::proc()
{
    edge=Mat::zeros(image.size(), CV_16SC1);
    double dE=2*eps;
    int i=0;
    while(dE>eps && i<max_iter_L)
    {
        ///2)-----------------mimimaze energi functional -------------------------
        minimaze_energi_fun();
        ///3.a.b)-----------------compute error  -------------------------
        compute_error();
        std::cout<<sigma<<std::endl;
        tresh=0.1*sigma;
        ///3.c)-----------------compute control signal  -------------------------
        calculateControlSignal();
        ///3.d)-----------------compute magnutude of zero crossing in error signal zce(x,y)  -------------------------
        calculateZCE();
        ///4)------------------ update edges ------------------------------------------
        Mat E_old=edge.clone();
        updateEdge();
        ///5)------------------ update lambda -----------------------------------------
        updateLambda();
        dE=cv::norm(edge-E_old);
        std::cout<<i<<"|dE: "<<dE<<std::endl;
        cv::imwrite("IRR_"+std::to_string(i)+".jpg",trac(edge));
        i++;


    }
   // return trac(edge);
    return edge;
}
#include <iostream>
void IRR_filter::minimaze_energi_fun()
{
    Mat U_n=Mat::zeros(image.size(), CV_32FC1);
    int begin=1;
    double dU(2*eps);
    //double dU1,ddU(2*eps);

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
        std::cout<<i<<"|dU:"<<dU<<std::endl;
        dU=cv::norm(U_min-U_n);
        U_n=U_min.clone();

        i++;
    }
    std::cout<<i<<"|dU:"<<dU<<std::endl;
    std::cout<<"energi:"<<cv::norm(U_min)<<std::endl;
}
void IRR_filter::compute_error()
{
    error=Mat(image.rows, image.cols,CV_32F);
    error_abs=Mat(image.rows, image.cols,CV_32F);
    sigma=0;
    for(int y=0; y<image.rows; y++)
    {
        auto error_row = error.ptr<float>(y);
        auto error_abs_row =error_abs.ptr<float>(y);
        const short* image_row = image.ptr<short>(y);
        const float* U_min_row = U_min.ptr<float>(y);
        for(int x=0;x<image.cols;x++)
        {
            double error=U_min_row[x]-image_row[x];
            error_abs_row[x]=std::fabs(error);
            error_row[x]=error;
            sigma+=error;
        }
    }
    sigma/=(image.rows+1)*(image.cols+1);

}

void IRR_filter::calculateControlSignal()
{
    //std::cout<<cv::norm(error_abs,cv::NORM_L2)<<std::endl;
    cv::Mat A=conv_filter.apply(error_abs);
    cv::subtract(A, cv::Scalar(sigma), A);
    control_signal=conv_filter.apply(A);
   //std::cout<<cv::norm(control_signal,cv::NORM_L2)<<std::endl;
}
void IRR_filter::calculateZCE()
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
            float shr=error_row2[x+1];
            float shl=error_row2[x-1];
            float sht=error_row3[x];
            float shb=error_row1[x];
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
}
void IRR_filter::updateEdge()
{
    for(int y=0; y<image.rows; y++)
    {
        int* edge_row = edge.ptr<int>(y);
        const float* zce_row = zce.ptr<float>(y);
        const float* control_signal_row = control_signal.ptr<float>(y);
        for(int x=0; x<image.cols; x++)
        {
         //   std::cout<<x<<'|'<<y<<"|cs:"<<control_signal_row[x]<<std::endl;
            if(edge_row[x]==0  && zce_row[x]>tresh  && control_signal_row[x]>0)
            {
                edge_row[x]=static_cast<int>(zce_row[x]);
            }

        }
    }
}
double IRR_filter::alha(float x)
{
    double exp=std::exp(-x/V_t);
    return l_min*(1-exp)+x*(exp);
}

void IRR_filter::updateLambda()
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
