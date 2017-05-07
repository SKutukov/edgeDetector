#include <QCoreApplication>
#include "opencv2/core/core.hpp"
#include "opencv2/imgcodecs/imgcodecs.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "iostream"
#include "opencv2/opencv.hpp"
#include <iomanip>
#include <cmath>

using namespace std;
using namespace cv;

//This class is essentially a struct of 4 Kuwahara regions surrounding a pixel, along with each one's mean, sum and variance.
class Regions{
    int* Area[4];
    int Size[4];
    unsigned long long Sum[4];
    double Var[4];
    int kernel;
public:
    Regions(int _kernel) : kernel(_kernel) {
        for (int i = 0; i<4; i++) {
            Area[i] = new int[kernel*kernel];
            Size[i] = 0;
            Sum[i] = 0;
            Var[i] = 0.0;
        }
    }

    //Update data, increase the size of the area, update the sum
    void sendData(int area, int data){
        Area[area][Size[area]] = data;
        Sum[area] += data;
        Size[area]++;
    }
    //Calculate the variance of each area
    double var(int area) {
        int __mean = Sum[area]/Size[area];
        double temp = 0;
        for (int i = 0; i<Size[area]; i++) {
            temp+= (Area[area][i] - __mean) * (Area[area][i] - __mean);
        }
        if (Size[area]==1) return 1.7e38; //If there is only one pixel inside the region then return the maximum of double
                                           //So that with this big number, the region will never be considered in the below minVar()
        return sqrt(temp/(Size[area]-1));
    }
    //Call the above function to calc the variances of all 4 areas
    void calcVar() {
        for (int i = 0; i<4; i++) {
            Var[i] = var(i);
        }
    }
    //Find out which regions has the least variance
    int minVar() {
        calcVar();
        int i = 0;
        double __var = Var[0];
        if (__var > Var[1]) {__var = Var[1]; i = 1;}
        if (__var > Var[2]) {__var = Var[2]; i = 2;}
        if (__var > Var[3]) {__var = Var[3]; i = 3;}
        return i;
    }

    //Return the mean of that regions
    uchar result(){
        int i = minVar();
        return saturate_cast<uchar> ((double) (Sum[i] *1.0 / Size[i]));
    }
};

class Kuwahara{
private:
    int wid, hei, pad, kernel;
    Mat image;
public:
    Regions getRegions(int x, int y){
        Regions regions(kernel);

        uchar *data = image.data;

        //Update data for each region, pixels that are outside the image's boundary will be ignored.

        //Area 1 (upper left)
        for (int j = (y-pad >=0)? y-pad : 0; j>= 0 && j<=y && j<hei; j++)
            for (int i = ((x-pad >=0) ? x-pad : 0); i>= 0 && i<=x && i<wid; i++) {
                regions.sendData(1,data[(j*wid)+i]);
            }
        //Area 2 (upper right)
        for (int j = (y-pad >=0)? y-pad : 0; j<=y && j<hei; j++)
            for (int i = x; i<=x+pad && i<wid; i++) {
                regions.sendData(2,data[(j*wid)+i]);
            }
        //Area 3 (bottom left)
        for (int j = y; j<=y+pad && j<hei; j++)
            for (int i = ((x-pad >=0) ? x-pad : 0); i<=x && i<wid; i++) {
                regions.sendData(3,data[(j*wid)+i]);
            }
        //Area 0 (bottom right)
        for (int j = y; j<=y+pad && j<hei; j++)
            for (int i = x; i<=x+pad && i<wid; i++) {
                regions.sendData(0,data[(j*wid)+i]);
            }
        return regions;
    }

    //Constructor
    Kuwahara(const Mat& _image, int _kernel) : kernel(_kernel) {
        image = _image.clone();
        wid = image.cols; hei = image.rows;
        pad = kernel-1;
    }

    //Create new image and replace its pixels by the results of Kuwahara filter on the original pixels
    Mat apply(){
        Mat temp;
        temp.create(image.size(), CV_8U);
        uchar* data = temp.data;

        for (int j= 0; j<hei; j++) {
            std::cout<<((double)j)/hei<<std::endl;
            for (int i = 0; i<wid; i++)
                data[j*wid+i] = getRegions(i,j).result();
        }
        return temp;
    }
};

int main() {
    Mat img = imread("/home/skutukov/Documents/zenith.png", 1);
    Mat gray, dest;
    int kernel = 5;
    gray.create(img.size(), CV_8U);
    cvtColor(img, gray, CV_BGR2GRAY);


    Kuwahara filter(gray, kernel);

    dest = filter.apply();
    //cv::Canny(dest,dest,35,90);
   // imshow("Result", dest);
    imwrite("result.jpg", dest);
    //waitKey();
}

//int main(int argc, char *argv[])
//{
//    IplImage* image = 0;
//    IplImage* dst = 0;
//    image = cvLoadImage("/home/skutukov/Documents/cat.png",1);

//    // окно для отображения картинки
//    cvNamedWindow("original",CV_WINDOW_AUTOSIZE);
//    dst = kuwahara_grey2(image,5);
//    // показываем картинку
//    cvShowImage("original",dst);
//    //cvSaveImage("./res.jpg", dst);

//    // ждём нажатия клавиши
//    cvWaitKey(0);

//    // освобождаем ресурсы
//    cvReleaseImage(& image);
//    // удаляем окно
//    cvDestroyWindow("original");
//    return 0;

////    cv::Mat src=cv::imread("/home/skutukov/Documents/cat.png");
////    cv::Mat out;
////    cv::Mat bgr[3];   //destination array
////    split(src,bgr);//split source
////    cv::Mat B(src.size(),CV_8UC1);
////    cv::Mat G(src.size(),CV_8UC1);
////    cv::Mat R(src.size(),CV_8UC1);
////    kuwahara_grey(bgr[0],B,5);
////    kuwahara_grey(bgr[1],G,5);
////    kuwahara_grey(bgr[2],R,5);
////    std::vector<cv::Mat> array_to_merge;
////    array_to_merge.push_back(B);
////    array_to_merge.push_back(G);
////    array_to_merge.push_back(R);
////    cv::merge(array_to_merge, out);
////    cv::Mat dest;
////    cv::Canny(out,dest,90,255);
////    cv::imwrite("./res.jpg",out);
////    cv::imwrite("./edges.jpg",dest);
//}
