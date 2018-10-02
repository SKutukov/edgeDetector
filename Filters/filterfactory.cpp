#include "filterfactory.h"

Filter FilterFactory::createFilter(filterType type)
{
    switch (type) {
        case SOBEL:
            return Sobel();
            break;
        case CANNY:
            return Canny();
            break;
        case LAPLAS:
            return Laplas();
            break;
        case IRR_FILTER:
            return IRR();
            break;
    }

}

FilterFactory::FilterFactory()
{

}
