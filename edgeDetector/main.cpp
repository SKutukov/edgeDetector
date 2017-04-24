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
int main(int argc, char *argv[])
{
    auto func = [] (double error)
    {
        cv::Mat dest;
        cv::Mat image = cv::imread("/home/skutukov/Pictures/Поляризационный_фильтр/1/outNew(1.600000)(0.000000_45.000000_90.000000)_zenith.png",CV_LOAD_IMAGE_ANYDEPTH);
        procesing(image,dest,35,90,IRR,error);
        //cv::imshow("test",dest);
        //cv::waitKey(0);
        cv::imwrite("result/res_"+std::to_string(error)+".jpg",dest);
     };
    double error(10);
    for(int i=0;i<5;i++)
    {
        func(error);
        error/=10;
    }
//    main_proc(argc,argv);
}

/**
 * @brief Threshold_Demo
 */
void Threshold_Demo( int, void* )
{
    procesing(src_gray,dst,threshold_value,threshold_max_value,CANNY);
    cv::imshow( window_name, dst );

}
void main_proc(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("Edge Detector");
    QCoreApplication::setApplicationVersion("0.1.0");
    ////-----------------prepare Command line parser —----------------------------------
    QCommandLineParser parser;
    QString str="Mode of blending: \n NO — no blending \n LINEAL — lineal blending \n MULTI — multiblending";
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
    QString input_directory="/home/skutukov/Pictures/sources/";
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
                            cv::Mat dest;
                            cv::Mat image = cv::imread(filename.toStdString(),CV_LOAD_IMAGE_ANYDEPTH);
                            procesing(image,dest,threshold_value,threshold_max_value,CANNY);
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
