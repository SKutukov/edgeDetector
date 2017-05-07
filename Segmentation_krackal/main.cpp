#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/photo/photo.hpp"
#include "vector"
#include "iostream"
using namespace cv;
using namespace std;
struct A
{
    A()
    {
        i=0;
        j=0;
    }

    A(int i,int j)
    {
       this->i=i;
       this->j=j;
    }
    int i,j;
};

Mat res;
Mat src;
vector<vector<int>> nodes;

vector<vector<vector<A>>> edg;
cv::Vec3b color;
void dfs(int& i,int& j,vector<vector<bool>>& used)
{
        used[i][j]=true;
        for(int k=0;k<edg[i][j].size();k++)
        {
            int m=edg[i][j][k].i;
            int s=edg[i][j][k].j;
            if(!used[m][s])
            {
                res.at<cv::Vec3b>(m,s)=color;
                dfs(m,s,used);
            }
        }



}
int main(int argc, char *argv[])
{

    Mat src=imread("/home/skutukov/test/beach.png",0);
    Rect ROI(0,0,20,20);
    //src=src(ROI);
    if(!src.data)
    {
        return -1;
    }

    //cv::fastNlMeansDenoising(src, src, 2);
    //cv::resize(src,src,Size(300,300));
    //cvtColor(src, src, CV_BGR2GRAY);
    res=Mat(src.size(),CV_8UC3);
    ///-------------------------------------------
    vector<vector<bool>> used;
    vector<int> a;
    vector<bool> u;
    vector<vector<A>> C;
    for(int j=0;j<src.cols;j++)
    {
        u.push_back(false);
        a.push_back(j);
        C.push_back(vector<A>(0));
    }
    for(int i=0;i<src.rows;i++)
    {
        nodes.push_back(a);
        used.push_back(u);
        edg.push_back(C);
    }

    ///--------------------------------------------------
    int thr=7;
    for(int i=0;i<src.rows;i++)
    {
        for(int j=0;j<src.cols;j++)
            {
                if((std::abs(src.at<uchar>(i,j)-src.at<uchar>(i-1,j))<thr)&&(i>1))
                {
                    edg[i][j].push_back(A(i-1,j));
                }
                if((std::abs(src.at<uchar>(i,j)-src.at<uchar>(i+1,j))<thr)&&(i<src.rows-1))
                {
                     edg[i][j].push_back(A(i+1,j));
                }
                if((std::abs(src.at<uchar>(i,j)-src.at<uchar>(i,j+1))<thr)&&(j<src.cols-1))
                {
                     edg[i][j].push_back(A(i,j+1));
                }
               if((std::abs(src.at<uchar>(i,j)-src.at<uchar>(i,j-1))<thr)&&(j>1))
                {
                     edg[i][j].push_back(A(i,j-1));
                }
            }
    }

    for(int i=0;i<src.rows;i++)
    {
        for(int j=0;j<src.cols;j++)
        {
            int b = cv::theRNG().uniform(0, 255);
            int g = cv::theRNG().uniform(0, 255);
            int r = cv::theRNG().uniform(0, 255);
            color=cv::Vec3b((uchar)b, (uchar)g, (uchar)r);
            if(!used[i][j])
            {
                dfs(i,j,used);
            }

        }
    }


    cv::imwrite("./res.jpg",res);

}

