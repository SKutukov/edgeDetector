#include "gui.h"
#include "processing.h"

GUI::GUI()
{

}
/**
 * @brief Threshold_Demo
 */
void Threshold_Demo(int, void* )
{
    dst=procesing(src_gray, threshold_value, threshold_max_value, CANNY, 20.);
    cv::imshow( window_name, dst );

}
void GUI::run(const std::string& filename)
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
    Threshold_Demo(0,0);
    /// Wait until user finishes program
    while(true)
    {
      int c;
      c = cv::waitKey( 20 );
      if( (char)c == 27 )
      {
          cv::imwrite(filename, dst );
          break;
      }
    }
}
