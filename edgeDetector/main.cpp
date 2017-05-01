#include "string"
#include "iostream"
#include "processing.h"
#include "QCoreApplication"
#include <QDirIterator>
#include <QCommandLineParser>
#include <tbb/parallel_for.h>

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
int main(int argc, char *argv[])
{
    auto func = [] (double error)
    {

        cv::Mat image = cv::imread("/home/skutukov/Pictures/filter/3/IMG90.jpg",CV_LOAD_IMAGE_ANYDEPTH);
        cv::Mat dest=procesing(image,35,90,CANNY,error);
        //cv::imshow("test",dest);
        //cv::waitKey(0);
//        std::vector<vector<cv::Point> > contours;
//        std::vector<Vec4i> hierarchy;
//        cv::findContours( dest, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
//        /// Draw contours
//        cv::Mat drawing = cv::Mat::zeros( dest.size(), CV_8UC3 );
//        for( int i = 0; i< contours.size(); i++ )
//           {
//             cv::Scalar color = cv::Scalar( 0, 0, 255 );
//             cv::drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
//           }
//        dest=drawing
        cv::imwrite("./result/IMG90.jpg",dest);
     };
    double error(0.1);
    ///for(int i=0;i<5;i++)
    //{
        func(error);
    //}
 //    main_proc(argc,argv);
}

/**
 * @brief Threshold_Demo
 */
void Threshold_Demo( int, void* )
{
    dst=procesing(src_gray,threshold_value,threshold_max_value,CANNY,20.);
    cv::imshow( window_name, dst );

}
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
    std::string output_directory="result/";
    if(parser.isSet(outputs_dir))
    {
        output_directory=parser.value(outputs_dir).toStdString();
    }
    /// ----------------------- get input directory -----------------------
    QString input_directory="/home/skutukov/Pictures/filter/1";
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

        tbb::parallel_for (size_t(0), sources.size(), size_t(1),
                        [=](size_t i)
                        {
                            QString filename=sources[i];
                            auto tmp=func(filename,"/");
                            std::cout<<"Procesing "<< filename.toStdString() <<std::endl;
                            //----------------- procesing ---------------------------

                            cv::Mat image = cv::imread(filename.toStdString(),CV_LOAD_IMAGE_ANYDEPTH);
                            cv::Mat dest=procesing(image,threshold_value,threshold_max_value,CANNY);
                            //---------------write result -----------------------------------
                            std::string output_name=output_directory+tmp.toStdString();
                            cv::imwrite(output_name, dest);
                        });
    }
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
