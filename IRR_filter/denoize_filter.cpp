#include "denoize_filter.h"

denoize_filter::denoize_filter(float force)
{
    this->force=force;
}
void denoize_filter::apply(Mat src,Mat dst)
{
    //cv::fastNlMeansDenoisingColored(src, dst, force);
}

