#include "denoize_filter.h"
#include "auxiliary_function.h"
#include "iostream"
denoize_filter::denoize_filter(float force)
{
    this->force=force;
}
void denoize_filter::apply(Mat src,Mat dst)
{
//    Mat U=src.clone();
//    Mat U_n=src.clone();
//    double dU(2*eps);
//    int i=0;
//    while(dU>eps && i<20000)
//    {
//        double dx,dy,D;
//        for(int y=1;y<U.rows-1;y++)
//        {
//            const short* U_n_row1 = U_n.ptr<short>(y-1);
//            const short* U_n_row2 = U_n.ptr<short>(y);
//            const short* U_n_row3 = U_n.ptr<short>(y+1);
//                for(int x=1;x<U.cols-1;y++)
//                {
//                    dx=(U_n_row2[x+1]-U_n_row2[x-1])/2;
//                    dy=(U_n_row3[x]-U_n_row1[x])/2;
//                    D=std::sqrt(dx*dx+dy*dy);
//                    U.at<short>(y,x)=1-tay*(1)+tay*(1);
//                }
//        }
//        dU=avarege_error(U,U_n);
//  //      std::cout<<i<<"|dU: "<<dU<<" ddU:"<<ddU<<' '<<eps<<' '<<(ddU>eps)<<std::endl;
//        U_n=U.clone();

//        i++;
//    }
//    std::cout<<i<<"|dU:"<<dU<<std::endl;
//    std::cout<<i<<std::endl;
//    dst=U.clone();
}

