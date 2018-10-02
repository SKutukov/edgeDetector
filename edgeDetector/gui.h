#ifndef GUI_H
#define GUI_H

#include <string>

class GUI_app
{
public:
    GUI_app();
    void run();
private:
    cv::Mat src, src_gray, dst;
    int threshold_value = 35;
    int threshold_max_value = 255;
    int proceding_type = 3;
    int const max_value = 255;
    int const max_type = 2;
    int const max_BINARY_value = 255;

    const std::string window_name = "Edge detector";
    const std::string trackbar_type = "Type: \n 0: Sobel \n 1: Canny \n ";
    const std::string trackbar_value = "Value";
    const std::string trackbar_max_value = "Max value";

    /// Function headers
    void Threshold_Demo( int, void* );
};

#endif // GUI_H
