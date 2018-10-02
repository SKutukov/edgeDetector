#include <string>
#include <iostream>
#include <QCoreApplication>
#include <QCommandLineParser>

#include <algorithm>
#include "gui.h"
#include "consol_app.h"
#include "processing.h"
#include "filterfactory.h"

void main_proc(int argc, char *argv[]);

const QString STD_PATH="./sources/";    

double force=0;
double err=0.01;
int main(int argc, char *argv[])
{
    auto func = [] (double error)
    {
        cv::Mat image = cv::imread("/home/skutukov/Pictures/toReport/src/1.jpg", CV_LOAD_IMAGE_COLOR);
        cv::Mat dest=procesing(image, 35, 90, IRR_FILTER , error, 5. ,true);
        cv::imwrite("./res.jpg",dest);
     };
     main_proc(argc,argv);
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


    QString input_directory="/home/skutukov/Pictures/toReport/src";
    std::string output_directory="/home/skutukov/Pictures/toReport/IRR/";
    int threshold_value, threshold_max_value;

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
    if(!isGui)
    {

    } // end console mode
    if(isGui)
    {
        GUI_app gui = GUI_app();
        gui.run();
    }

}
