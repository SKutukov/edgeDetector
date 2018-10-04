#include "QCoreApplication"
#include "iostream"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "true_false_metric.h"
#include <QCommandLineParser>
#include <QDirIterator>
#include <memory>
#include <string>
#include <tbb/parallel_for.h>

void print_result(std::shared_ptr<True_false_Metric> met)
{
    std::cout << "Avarage: " << std::endl;
    double avarage_precision = static_cast<double>(met->precision) / met->size;
    double avarage_recall = static_cast<double>(met->recall) / met->size;
    std::cout << avarage_precision << ' ' << avarage_recall << std::endl;
    std::cout << (1 + met->b * met->b) * avarage_precision
                     * (1 - avarage_recall)
                     / (1 - avarage_recall
                        + met->b * met->b * avarage_precision)
              << std::endl;
    std::cout << met->f / met->size << std::endl;
}
int main(int argc, char **argv)
{
    QString input_dir_test = "/home/skutukov/Pictures/toReport/hv";
    QString input_directory = "/home/skutukov/Pictures/toReport/BIL";
    std::string output_directory = "/home/skutukov/Pictures/toReport/MS";

    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName("Test Edge Detector");
    QCoreApplication::setApplicationVersion("0.1.0");

    ////-----------------prepare Command line parser
    ///—----------------------------------
    QCommandLineParser parser;
    QString str = "";
    parser.setApplicationDescription(str);
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption inputs_dir(
        QStringList() << "i"
                      << "inputs_dir",
        QCoreApplication::translate("main", "Directory of source images"),
        QCoreApplication::translate("main", "directory"));
    parser.addOption(inputs_dir);

    QCommandLineOption inputs_dir_test(
        QStringList() << "s"
                      << "inputs_dir_test",
        QCoreApplication::translate("main", "Directory of test images"),
        QCoreApplication::translate("main", "directory"));
    parser.addOption(inputs_dir_test);

    QCommandLineOption outputs_dir(
        QStringList() << "o"
                      << "outputs_dir",
        QCoreApplication::translate("main",
                                    "Directory where result will be saved."),
        QCoreApplication::translate("main", "directory"));
    parser.addOption(outputs_dir);
    parser.process(a);

    /// ----------------------- get output directory -----------------------
    if (parser.isSet(outputs_dir)) {
        output_directory = parser.value(outputs_dir).toStdString();
    }
    /// ----------------------- get input directory -----------------------

    if (parser.isSet(inputs_dir)) {
        input_directory = parser.value(inputs_dir);
    }
    /// ----------------------- get input directory -----------------------

    if (parser.isSet(inputs_dir)) {
        input_dir_test = parser.value(inputs_dir);
    }
    /// ------------------ run test -------------------------------
    QDirIterator it(input_dir_test, QDirIterator::NoIteratorFlags);
    std::vector<QString> sources;
    auto func = [](QString str, QString delimiter) {
        auto tmpList = str.split(delimiter);
        return tmpList.value(tmpList.length() - 1);
    };
    while (it.hasNext()) {
        QString filename = it.next();
        auto tmp = func(filename, "/");
        auto ext = func(tmp, ".");
        if (ext.toStdString() == "jpg" || ext.toStdString() == "png") {
            sources.push_back(filename);
        }
    }
    static std::shared_ptr<True_false_Metric> met;
    tbb::parallel_for(size_t(0), sources.size(), size_t(1), [=](size_t i) {
        QString filename = sources[i];
        std::string tmp = func(filename, "/").toStdString();
        std::cout << "Procesing " << filename.toStdString() << std::endl;
        //----------------- procesing ---------------------------
        cv::Mat test = cv::imread(input_dir_test.toStdString() + "/" + tmp,
                                  CV_LOAD_IMAGE_GRAYSCALE);
        cv::Mat src = cv::imread(input_directory.toStdString() + "/" + tmp,
                                 CV_LOAD_IMAGE_GRAYSCALE);
        met->apply(test, src);
    });
    print_result(met);
}
