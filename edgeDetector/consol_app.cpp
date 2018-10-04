#include "consol_app.h"
#include "iostream"
#include "processing.h"
#include <QDirIterator>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <tbb/parallel_for.h>

consol_app::consol_app(int threshold, int threshold_max)
    : threshold_value(threshold), threshold_max_value(threshold_max)
{
}

void consol_app::run(const QString &input_directory,
                     const std::string &output_directory)
{
    QDirIterator it(input_directory, QDirIterator::NoIteratorFlags);
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
    ///-----------------------run filter ---------------------------
    tbb::parallel_for(size_t(0), sources.size(), size_t(1), [=](size_t i) {
        QString filename = sources[i];
        auto tmp = func(filename, "/");
        std::cout << "Procesing " << filename.toStdString() << std::endl;
        //----------------- procesing ---------------------------

        cv::Mat image
            = cv::imread(filename.toStdString().c_str(), CV_LOAD_IMAGE_COLOR);
        cv::Mat dest = procesing(image, threshold_value, threshold_max_value,
                                 type, force, true);
        //---------------write result -----------------------------------
        std::cout << "Writing " << output_directory + tmp.toStdString()
                  << std::endl;
        std::string output_name = output_directory + tmp.toStdString();
        cv::imwrite(output_name, dest);
    });
}
