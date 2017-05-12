#include <string>
#include <QDirIterator>
#include <QCommandLineParser>
#include "QCoreApplication"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "iostream"
#include <tbb/parallel_for.h>
#include "true_false_metric.h"

QString input_dir_test="/home/skutukov/Pictures/test";
QString input_directory="/home/skutukov/Pictures/sources";
std::string output_directory="/home/skutukov/Pictures/IRR/";

int main(int argc, char **argv)
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
    QCommandLineOption inputs_dir_test(QStringList() <<"s" << "inputs_dir_test",
                QCoreApplication::translate("main", "Directory of test images"),
                QCoreApplication::translate("main", "directory"));
    parser.addOption(inputs_dir_test);
    QCommandLineOption outputs_dir(QStringList() << "o" << "outputs_dir",
                QCoreApplication::translate("main", "Directory where result will be saved."),
                QCoreApplication::translate("main", "directory"));
    parser.addOption(outputs_dir);
    parser.process(a);

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
    /// ----------------------- get input directory -----------------------

    if(parser.isSet(inputs_dir))
    {
        input_dir_test=parser.value(inputs_dir);
    }
    /// ------------------ run test -------------------------------
    QDirIterator it(input_dir_test, QDirIterator::NoIteratorFlags);
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
                      std::string tmp=func(filename,"/").toStdString();
                      std::cout<<"Procesing "<< filename.toStdString() <<std::endl;
                      //----------------- procesing ---------------------------
                      cv::Mat test = cv::imread(input_dir_test.toStdString()+"/"+tmp,CV_LOAD_IMAGE_GRAYSCALE);
                      cv::Mat src = cv::imread(input_directory.toStdString()+"/"+tmp,CV_LOAD_IMAGE_GRAYSCALE);
                     True_false_Metric::apply(test,src);

                  });
//    ::testing::InitGoogleTest(&argc, argv);
//    return RUN_ALL_TESTS();
}
