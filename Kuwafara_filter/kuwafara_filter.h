#ifndef KUWAFARA_FILTER_H
#define KUWAFARA_FILTER_H

#include "opencv2/core/core.hpp"
#include "opencv2/imgcodecs/imgcodecs.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "array"
#include "vector"
using namespace std;
using namespace cv;
class Region{
    vector<uchar> Area;
    unsigned long long mean=0;
    double var=0.0;
    int kernel;
    Region(int _kernel) : kernel(_kernel)
    {
          Area.resize(_kernel*_kernel);
    }
public:
    std::array<Region,4> static getRegions(int kernel_size)
    {
         std::array<Region,4> res{{Region(kernel_size),
                                 Region(kernel_size),
                                 Region(kernel_size),
                                 Region(kernel_size)}};
         return res;

    }

    //Update data, increase the size of the area, update the sum
    void sendData(uchar data)
    {
        Area.push_back(data);
        mean += data;
    }
    //Calculate the variance of each area
    double Var()
    {
        mean /= Area.size();
        double temp = 0;
        for (uchar a : Area)
        {
            temp+= (a - mean) * (a - mean);
        }
        if (Area.size()==1) return 1.7e38; //If there is only one pixel inside the region then return the maximum of double
                                           //So that with this big number, the region will never be considered in the below minVar()
        return sqrt(temp/(Area.size()-1));
    }
    uchar Mean()
    {
        return static_cast<uchar>(mean);
    }
};

class Kuwahara{

public:
    Kuwahara(const Mat& _image, int _kernel);
    void  apply(Mat& dest);
private:
    int width, height, pad, kernel_size;
    Mat image;
    uchar getMean(int x, int y);


};

#endif // KUWAFARA_FILTER_H
