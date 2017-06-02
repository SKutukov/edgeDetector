#include "string"
#include "iostream"
#include "processing.h"
#include "QCoreApplication"
#include <QDirIterator>
#include <QCommandLineParser>
#include <tbb/parallel_for.h>
#include "algorithm"
/// Global variables
int threshold_value = 35;
int threshold_max_value = 255;
int proceding_type = 3;
int const max_value = 255;
int const max_type = 2;
int const max_BINARY_value = 255;

cv::Mat src, src_gray, dst;
std::string window_name = "Edge detector";

std::string trackbar_type = "Type: \n 0: Sobel \n 1: Canny \n ";
std::string trackbar_value = "Value";
std::string trackbar_max_value = "Max value";

/// Function headers
void Threshold_Demo( int, void* );
void main_proc(int argc, char *argv[]);


const QString STD_PATH="./sources/";    
using namespace cv;
using namespace std;

/**
 * @brief Threshold_Demo
 */
void Threshold_Demo( int, void* )
{
    dst=procesing(src_gray, threshold_value, threshold_max_value, CANNY, 20.);
    cv::imshow( window_name, dst );

}
//Mat trac(cv::Mat src)
//{
//    std::vector<std::vector<cv::Point> > contours;
//    std::vector<cv::Vec4i> hierarchy;
//    cv::findContours( src, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0) );

//    /// Draw contours
//    Mat drawing = Mat::zeros( src.size(), CV_8UC3 );
//    for( int i = 0; i< contours.size(); i++ )
//       {
//         cv::Scalar color = cv::Scalar(255, 255, 255);
//         drawContours( drawing, contours, i, color, 0.1, 8, hierarchy, 0, cv::Point() );
//       }
//    return drawing;
//}
double force=0;
double err=0.01;
int main(int argc, char *argv[])
{
    auto func = [] (double error)
    {

        cv::Mat image = cv::imread("/home/skutukov/Pictures/toReport/src/1.jpg", CV_LOAD_IMAGE_COLOR);
        cv::Mat dest=procesing(image, 35, 90, IRR , error, 5. ,true);
        //cv::imshow("test",dest);
        //cv::waitKey(0);
        cv::imwrite("./res.jpg",dest);
     };
 //
//        func(err);
       main_proc(argc,argv);
}
procType type=IRR;
QString input_directory="/home/skutukov/Pictures/toReport/src";
std::string output_directory="/home/skutukov/Pictures/toReport/IRR/";

void main_proc(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("Edge Detector");
    QCoreApplication::setApplicationVersion("0.1.0");
    ////-----------------prepare Command line parser —----------------------------------
    QCommandLineParser parser;
    QString str="";
    parser.setApplicationDescription(str);
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption inputs_dir(QStringList() <<"i" << "inputs_dir",
                QCoreApplication::translate("main", "Directory of source images"),
                QCoreApplication::translate("main", "directory"));
    parser.addOption(inputs_dir);
    QCommandLineOption outputs_dir(QStringList() << "o" << "outputs_dir",
                QCoreApplication::translate("main", "Directory where result will be saved."),
                QCoreApplication::translate("main", "directory"));
    parser.addOption(outputs_dir);
    QCommandLineOption threh(QStringList() << "threh" << "threh",
                QCoreApplication::translate("main", "threh"),
                QCoreApplication::translate("main", "int"));
    parser.addOption(threh);
    QCommandLineOption threh_max(QStringList() << "threh_max" << "threh_max",
                QCoreApplication::translate("main", "threh_max"),
                QCoreApplication::translate("main", "int"));
    parser.addOption(threh_max);
    QCommandLineOption mode_flag(QStringList() << "m" << "mode_flag",
                QCoreApplication::translate("main", "Flag for choosing edge detection mode"),
                QCoreApplication::translate("main", "NO/LINEAL/MULTI"));
    parser.addOption(mode_flag);
    QCommandLineOption gui_flag(QStringList() << "g" << "gui_flag",
                QCoreApplication::translate("main", "Flag for gui mode"),
                QCoreApplication::translate("main", "NO/YES"));
    parser.addOption(gui_flag);

    parser.process(a);
    /// ----------------------- get gui mode -----------------------
    bool isGui=false;
    if(parser.isSet(gui_flag))
    {
        if(parser.value(gui_flag)=="YES")
        {
            isGui=true;
        }
    }
    /// ----------------------- get output directory -----------------------
    if(parser.isSet(outputs_dir))
    {
        output_directory=parser.value(outputs_dir).toStdString();
    }
    /// ----------------------- get input directory -----------------------

    if(parser.isSet(inputs_dir))
    {
        input_directory=parser.value(inputs_dir);
    }
    /// ----------------------- get threshold_value -----------------------
    if(parser.isSet(threh))
    {
        threshold_value=std::stoi(parser.value(threh).toStdString());
    }
    /// ----------------------- get input directory -----------------------
    if(parser.isSet(threh_max))
    {
        threshold_max_value=std::stoi(parser.value(threh_max).toStdString());
    }
    a.exit();
    ///-------------- load threh and run concol mode  or run gui mode   ----------------------------------
    if((!isGui))
    {
        QDirIterator it(input_directory, QDirIterator::NoIteratorFlags);
        std::vector<QString> sources;
        auto func = [] (QString str,QString delimiter)
           {
            auto tmpList=str.split(delimiter);
            return tmpList.value(tmpList.length()-1);
        };
        while (it.hasNext())
        {
            QString filename=it.next();
            auto tmp=func(filename,"/");
            auto ext=func(tmp,".");
            if(ext.toStdString()=="jpg" || ext.toStdString()=="png")
            {
                sources.push_back(filename);

            }
        }
      ///------------------------sort -----------------------------
      //std::sort(sources.begin(),sources.back());
      ///-----------------------run filter ---------------------------
      tbb::parallel_for (size_t(0), sources.size(), size_t(1),
                      [=](size_t i)
  //      for(int i=0;i<sources.size();i++)
                      {
                          QString filename=sources[i];
                          auto tmp=func(filename,"/");
                          std::cout<<"Procesing "<< filename.toStdString() <<std::endl;
                          //----------------- procesing ---------------------------

                          cv::Mat image = cv::imread(filename.toStdString(),CV_LOAD_IMAGE_COLOR);
                          cv::Mat dest=procesing(image,threshold_value,threshold_max_value,type,err,force,true);
                          //---------------write result -----------------------------------
                          std::cout<<"Writing "<< output_directory+tmp.toStdString() <<std::endl;
                          std::string output_name=output_directory+tmp.toStdString();
                          cv::imwrite(output_name, dest);
                      });
   // }
    if(isGui)
    {
        // Load an image
        src = cv::imread(input_directory.toStdString(),1);
        // Convert the image to Gray
        cv::cvtColor( src, src_gray, CV_BGR2GRAY );
        // Create a window to display results
        cv::namedWindow( window_name, CV_WINDOW_AUTOSIZE );
        // Create Trackbar to choose type of Threshold
        cv::createTrackbar( trackbar_type,
                        window_name, &proceding_type,
                        max_type, Threshold_Demo );
        cv::createTrackbar( trackbar_value,
                        window_name, &threshold_value,
                        max_BINARY_value, Threshold_Demo );
        cv::createTrackbar( trackbar_max_value,
                        window_name, &threshold_max_value,
                        max_BINARY_value, Threshold_Demo );
        // Call the function to initialize
        Threshold_Demo( 0, 0 );
        /// Wait until user finishes program
        while(true)
        {
          int c;
          c = cv::waitKey( 20 );
          if( (char)c == 27 )
            {
     //         cv::imwrite( filename, dst );
              break;
          }
        }
    }
   }
}
