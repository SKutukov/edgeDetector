#include "kuwafara_filter.h"



Kuwahara::Kuwahara(const Mat& image, int _kernel)
    : kernel_size(_kernel),
      pad(kernel_size-1),
      width(image.cols),
      height(image.rows)
{
    this->image = image.clone();
}

#include "iostream"
void Kuwahara::apply(cv::Mat& dest)
{
    dest=cv::Mat(image.size(), CV_8U);
    uchar* data = dest.data;
    for (int y= 0; y<height; y++)
    {
        std::cout<<((double)y)/height<<std::endl;
        for (int x = 0; x<width; x++)
        {
            
            data[y*width+x] = getMean(x,y);
        }
    }

}


uchar Kuwahara::getMean(int x, int y)
{
    std::array<Region,4> regions = Region::getRegions(kernel_size);
    std::array<double,4> var={0,0,0,0};
    std::array<uchar,4> mean={0,0,0,0};

    uchar *data = image.data;
//    Area 1 (upper left)
    for (int j = (y-pad >=0)? y-pad : 0; j>= 0 && j<=y && j<height; j++)
    {
        for (int i = ((x-pad >=0) ? x-pad : 0); i>= 0 && i<=x && i<width; i++)
        {
            regions[0].sendData(data[(j*width)+i]);
        }
    }
    mean[0]=regions[0].Mean();
    var[0]=regions[0].Var();

    //Area 2 (upper right)
    for (int j = (y-pad >=0)? y-pad : 0; j<=y && j<height; j++)
        for (int i = x; i<=x+pad && i<width; i++)
        {
            regions[1].sendData(data[(j*width)+i]);
        }
    mean[1]=regions[1].Mean();
    var[1]=regions[1].Var();
    //Area 3 (bottom left)
    for (int j = y; j<=y+pad && j<height; j++)
        for (int i = ((x-pad >=0) ? x-pad : 0); i<=x && i<width; i++)
        {
            regions[2].sendData(data[(j*width)+i]);
        }
    mean[2]=regions[2].Mean();
    var[2]=regions[2].Var();
    //Area 0 (bottom right)
    for (int j = y; j<=y+pad && j<height; j++)
        for (int i = x; i<=x+pad && i<width; i++)
        {
            regions[3].sendData(data[(j*width)+i]);
        }
    mean[3]=regions[3].Mean();
    var[3]=regions[3].Var();

    int min_index = 0;
    double temp = var[0];
    for(int i=1;i<4;i++)
    {
        if (temp > var[i])
        {
            temp = var[i];
            min_index = i;
        }
    }

    return mean[min_index];
}
