#include "filterfactory.h"

std::shared_ptr<Filter> FilterFactory::createFilter(filterType type)
{
    switch (type) {
        case SOBEL:
            return  std::make_shared<Sobel>(Sobel());
        case CANNY:
            return std::make_shared<Canny>(Canny());
        case LAPLAS:
            return std::make_shared<Laplas>(Laplas());
        case IRR_FILTER:
            return std::make_shared<IRR>(IRR());
        case CONV:
            return std::make_shared<Convolution_filter>(Convolution_filter());
    }
}

FilterFactory::FilterFactory()
{

}
